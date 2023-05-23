#include "VRLoader.h"

VRLoader::VRLoader() {
//-----------------------------------------------------------------------------
// Purpose: Constructor.
//-----------------------------------------------------------------------------

	this->nearClip = 0.1f;
	this->farClip = 200.0f;
}

VRLoader::~VRLoader() {
//-----------------------------------------------------------------------------
// Purpose: Destructor.
//-----------------------------------------------------------------------------
}

std::string VRLoader::GetTrackedDeviceString(vr::TrackedDeviceIndex_t unDevice, vr::TrackedDeviceProperty prop, vr::TrackedPropertyError* peError = NULL)
{
//-----------------------------------------------------------------------------
// Purpose: Converts a device index to a device name in string format.
// 
// Returns: The string denoting this device's name (for loading the correct model)
//-----------------------------------------------------------------------------
	uint32_t unRequiredBufferLen = vr::VRSystem()->GetStringTrackedDeviceProperty(unDevice, prop, NULL, 0, peError);
	if (unRequiredBufferLen == 0)
		return "";

	char* pchBuffer = new char[unRequiredBufferLen];
	unRequiredBufferLen = vr::VRSystem()->GetStringTrackedDeviceProperty(unDevice, prop, pchBuffer, unRequiredBufferLen, peError);
	std::string sResult = pchBuffer;
	delete[] pchBuffer;
	return sResult;
}

glm::mat4 VRLoader::getEyeProjectionMatrix(vr::Hmd_Eye eye) {
//-----------------------------------------------------------------------------
// Purpose: Gets the projection matrix for the specified eye.
// 
// Returns: ProjectionMatrix with respect to the specified eye.
//-----------------------------------------------------------------------------
	vr::HmdMatrix44_t matrix = pHMD->GetProjectionMatrix(eye, nearClip, farClip);
	return glm::mat4(
		matrix.m[0][0], matrix.m[1][0], matrix.m[2][0], matrix.m[3][0],
		matrix.m[0][1], matrix.m[1][1], matrix.m[2][1], matrix.m[3][1],
		matrix.m[0][2], matrix.m[1][2], matrix.m[2][2], matrix.m[3][2],
		matrix.m[0][3], matrix.m[1][3], matrix.m[2][3], matrix.m[3][3]
	);
}

glm::mat4 VRLoader::getEyeViewMatrix(vr::Hmd_Eye eye) {
//-----------------------------------------------------------------------------
// Purpose: Gets the view matrix for the specified eye.
//
// Returns: ViewMatrix with respect to the specified eye.
//-----------------------------------------------------------------------------
	vr::HmdMatrix34_t matrix = pHMD->GetEyeToHeadTransform(eye);
	glm::mat4 mat4EyePose = glm::mat4(
		matrix.m[0][0], matrix.m[1][0], matrix.m[2][0], 0.0,
		matrix.m[0][1], matrix.m[1][1], matrix.m[2][1], 0.0,
		matrix.m[0][2], matrix.m[1][2], matrix.m[2][2], 0.0,
		matrix.m[0][3], matrix.m[1][3], matrix.m[2][3], 1.0
	);
	return inverse(mat4EyePose);
}

glm::mat4 VRLoader::getEyeViewProjectionMatrix(vr::Hmd_Eye eye) {
//-----------------------------------------------------------------------------
// Purpose: Gets the combined view and projection matrix for the specified eye.
//
// Returns: View and Projection Matrix with respect to the specified eye.
//-----------------------------------------------------------------------------

	glm::mat4 matrix;
	if (eye == vr::Eye_Left) {
		matrix = ProjectionMatrixLeftEye * ViewMatrixLeftEye * mat4HMDPose;

	}
	else if (eye == vr::Eye_Right) {
		matrix = ProjectionMatrixRightEye * ViewMatrixRightEye * mat4HMDPose;
	}

	return matrix;

}

glm::mat4 VRLoader::convertSteamVRMatrix(const vr::HmdMatrix34_t& pose) {
//-----------------------------------------------------------------------------
// Purpose: Converts a SteamVR matrix to a glm matrix.
//			The SteamVR matrix is row-major, while the glm matrix is column-major.
// 
// Returns: glm matrix.
//-----------------------------------------------------------------------------
	glm::mat4 matrix(
		pose.m[0][0], pose.m[1][0], pose.m[2][0], 0.0,
		pose.m[0][1], pose.m[1][1], pose.m[2][1], 0.0,
		pose.m[0][2], pose.m[1][2], pose.m[2][2], 0.0,
		pose.m[0][3], pose.m[1][3], pose.m[2][3], 1.0f
	);
	return matrix;
}

Model3D VRLoader::FindOrLoadRenderModel(const char * renderModelName) {
//-----------------------------------------------------------------------------
// Purpose: Finds a render model using the render model's name (renderModelname).
//
// Returns: The render model.
//-----------------------------------------------------------------------------
	vr::RenderModel_t* pModel;//holds information about the model
	vr::RenderModel_TextureMap_t * pTexture;//holds texture info of model
	vr::EVRRenderModelError error;
	//this is an async function, so we need to wait for it to finish loading (hence the while loop). The same goes for the textures.
	while (1) {
		error = vr::VRRenderModels()->LoadRenderModel_Async(renderModelName, &pModel);
		if (error != vr::VRRenderModelError_Loading) {
			break;
		}
	}

	while (1) {
		error = vr::VRRenderModels()->LoadTexture_Async( pModel->diffuseTextureId, &pTexture);
		if (error != vr::VRRenderModelError_Loading) {
			break;
		}
	}

	Model3D controller;
	controller.loadOpenVRModel(pModel, pTexture); // make a Model3D of the controller
	controller.compileModel(); //compile the model


	return controller;

}

void VRLoader::updateHMDPose() {
//-----------------------------------------------------------------------------
// Purpose: Updates the pose of the HMD.
// 
// Returns: N/A
//-----------------------------------------------------------------------------

// Calling WaitGetPoses gets the set of poses that are currently available, with an optional timeout of 0ms. In this case, that means we get the
// latest poses. If no poses are available, we'll just get the ones we got the last time through the loop. Waiting for poses is the key to
// getting good latency.
// The purpose of poses is to describe the offset of each device since the last frame. This lets you know how far each device has moved.


	vr::VRCompositor()->WaitGetPoses(trackedDevicePose, vr::k_unMaxTrackedDeviceCount, NULL, 0);

	int validPoseCount = 0;
	std::string poseClasses = "";

	//prints the strings of all connected devices, and updates their poses.
	for (int nDevice = 0; nDevice < vr::k_unMaxTrackedDeviceCount; ++nDevice) {
		if (trackedDevicePose[nDevice].bPoseIsValid) {
			validPoseCount++;
			mat4DevicePose[nDevice] = convertSteamVRMatrix(trackedDevicePose[nDevice].mDeviceToAbsoluteTracking);
			if (m_rDevClassChar[nDevice] == 0) {
				switch (pHMD->GetTrackedDeviceClass(nDevice))
				{
				case vr::TrackedDeviceClass_Controller:        m_rDevClassChar[nDevice] = 'C'; break;
				case vr::TrackedDeviceClass_HMD:               m_rDevClassChar[nDevice] = 'H'; break;
				case vr::TrackedDeviceClass_Invalid:           m_rDevClassChar[nDevice] = 'I'; break;
				case vr::TrackedDeviceClass_GenericTracker:    m_rDevClassChar[nDevice] = 'G'; break;
				case vr::TrackedDeviceClass_TrackingReference: m_rDevClassChar[nDevice] = 'T'; break;
				default:                                       m_rDevClassChar[nDevice] = '?'; break;
				}
			}
			poseClasses += m_rDevClassChar[nDevice];
		}
	}

	//get the pose of the HMD, and turns it to a GLM matrix. We can get the pose of the controllers in a similar way (Which is done elsewhere).
	this->mat4DevicePose[vr::k_unTrackedDeviceIndex_Hmd] = convertSteamVRMatrix(trackedDevicePose[vr::k_unTrackedDeviceIndex_Hmd].mDeviceToAbsoluteTracking);
	if (trackedDevicePose[vr::k_unTrackedDeviceIndex_Hmd].bPoseIsValid) {
		this->mat4HMDPose = mat4DevicePose[vr::k_unTrackedDeviceIndex_Hmd];
		this->mat4HMDPose = inverse(this->mat4HMDPose);
	}
}

void VRLoader::ProcessVREvent(const vr::VREvent_t& event)
{
	switch (event.eventType)
	{
	case vr::VREvent_TrackedDeviceDeactivated:
	{
		printf("Device %u detached.\n", event.trackedDeviceIndex);
	}
	break;
	case vr::VREvent_TrackedDeviceUpdated:
	{
		printf("Device %u updated.\n", event.trackedDeviceIndex);
	}
	break;
	}
}

void VRLoader::handleInput() {
//-----------------------------------------------------------------------------
// Purpose: Updates the model data for the controllers based on what the user 
//			is inputting, and loads the model if they are not already loaded.
// 
// Returns: N/A
//-----------------------------------------------------------------------------
	m_rHand[Left].m_bShowController = true;
	m_rHand[Right].m_bShowController = true;

	//a vr event happens when a vr device is attached or detached

	//this creates a VREvent_t object, and stores whatever event happens in the "event" object. This loops through every event that happens, and prints them using ProcessVREvent. (For our own benefit)
	vr::VREvent_t event;
	while (pHMD->PollNextEvent(&event, sizeof(event)))
	{
		ProcessVREvent(event);
	}
	vr::VRActiveActionSet_t actionSet = { 0 };//We think this is something to do with the inoput bindings. Code leftover from the sample code to get controllers working.
	actionSet.ulActionSet = m_actionsetDemo;
	vr::VRInput()->UpdateActionState(&actionSet, sizeof(actionSet), 1);

	//loops through each hand, casts the hand enum to an int, and loops through both enums. This is used to get the pose of each hand, and if it's active or not.
	for (EHand eHand = Left; eHand <= Right; ((int&)eHand)++)
	{
		vr::InputPoseActionData_t poseData;

		//we pass this the controllers pose data, and we get a filled pose data struct which holds info about the controller
		if (vr::VRInput()->GetPoseActionDataForNextFrame(m_rHand[eHand].m_actionPose, vr::TrackingUniverseStanding, &poseData, sizeof(poseData), vr::k_ulInvalidInputValueHandle) != vr::VRInputError_None
			|| !poseData.bActive || !poseData.pose.bPoseIsValid)//if this pose data returns an error, or the controller isnt on, dont show the controller
		{
			m_rHand[eHand].m_bShowController = false;
		}
		else //If we have no errors:
		{
			//get the transform of the controller - where the controller is compared to the headset (we think)
			m_rHand[eHand].m_rmat4Pose = convertSteamVRMatrix(poseData.pose.mDeviceToAbsoluteTracking);

			//get info about the device based off the pose data. poseData includes info about the current state of the controller, and we use this to see if its being tracked by the headset / base station - if its being tracxked its in view (we think)
			vr::InputOriginInfo_t originInfo;
			if (vr::VRInput()->GetOriginTrackedDeviceInfo(poseData.activeOrigin, &originInfo, sizeof(originInfo)) == vr::VRInputError_None
				&& originInfo.trackedDeviceIndex != vr::k_unTrackedDeviceIndexInvalid)
			{
				//There is a list of all devices which are connected. trackeddevice index is the index of THIS device in that list. We use this to get the name of the device, and if it's different to the last time we checked, we load a new model for it.
				std::string sRenderModelName = GetTrackedDeviceString(originInfo.trackedDeviceIndex, vr::Prop_RenderModelName_String);
				//Once we load a model name for the hand, we store the model name in the hand. once this changes, we need to update the model. this is so we dont reload the model every frame.
				if (sRenderModelName != m_rHand[eHand].m_sRenderModelName)
				{
					//set the render model in the hand
					m_rHand[eHand].m_pRenderModel = FindOrLoadRenderModel(sRenderModelName.c_str());
					std::cout << "Loaded: " << sRenderModelName << std::endl;
					m_rHand[eHand].m_sRenderModelName = sRenderModelName; //set the render name also. this is so the thing where we check the render model every frame so we dont have to re-load it works.
				}
			}
		}
	}
}

int VRLoader::initVR() {
//-----------------------------------------------------------------------------
// Purpose: Initialise OpenVR.
// 
// Returns: 1 or 0 depending on whether OpenVr started successfully or not.
//-----------------------------------------------------------------------------
	if (vr::VR_IsHmdPresent()) {
		std::cout << "HMD is present." << std::endl;
		if (vr::VR_IsRuntimeInstalled()) {
			std::cout << "OpenVR Runtime is installed." << std::endl;
		}
		else {
			std::cout << "OpenVR Runtime is not installed." << std::endl;
			return -1;
		}
	}
	else {
		std::cout << "HMD is not present." << std::endl;
		return -1;
	}

	//Load SteamVR Runtime

	vr::EVRInitError evrError = vr::VRInitError_None;
	//pHMD is our object for storing information about + doing commands the headset. Its basically as if the headset was an object in code.
	pHMD = vr::VR_Init(&evrError, vr::VRApplication_Scene);

	if (evrError != vr::VRInitError_None) {
		std::cout << "Could not load runtime." << std::endl;
		return -1;
	}

	vr::EVRInitError cError = vr::VRInitError_None;

	//compistor is used to render to the headsets screens.
	if (!vr::VRCompositor()) {
		std::cout << "Could not load Compositor." << std::endl;
		return -1;
	}

	//check controller keybinding files loads properly
	vr::EVRInputError error = vr::VRInputError_None;


	TCHAR path_buffer[MAX_PATH];
	GetModuleFileNameW(NULL, path_buffer, MAX_PATH);
	std::wstring path(path_buffer);
	//path.substr(0, path.find_last_of(L"\\/"));
	std::wcout << path << std::endl;


	error = vr::VRInput()->SetActionManifestPath("C:/Users/Eleva/Documents/GitHub/MComp/DockItUI/src/hellovr_actions.json");
	//error = vr::VRInput()->SetActionManifestPath("C:/Users/Will/Documents/GitHub/MComp/DockItUI/src/hellovr_actions.json");
	//std::cout << "error: " << error << std::endl;

	//get the keybindings, and store them in the action handles. Mainly used for bug fixing at the moment - we are still working on figuring out this codes exact use, but our program works when its here.
	vr::VRInput()->GetActionHandle("/actions/demo/in/HideCubes", &m_actionHideCubes);
	vr::VRInput()->GetActionHandle("/actions/demo/in/HideThisController", &m_actionHideThisController);
	vr::VRInput()->GetActionHandle("/actions/demo/in/TriggerHaptic", &m_actionTriggerHaptic);
	vr::VRInput()->GetActionHandle("/actions/demo/in/AnalogInput", &m_actionAnalongInput);

	vr::VRInput()->GetActionSetHandle("/actions/demo", &m_actionsetDemo);

	vr::VRInput()->GetActionHandle("/actions/demo/out/Haptic_Left", &m_rHand[Left].m_actionHaptic);
	vr::VRInput()->GetInputSourceHandle("/user/hand/left", &m_rHand[Left].m_source);
	vr::VRInput()->GetActionHandle("/actions/demo/in/Hand_Left", &m_rHand[Left].m_actionPose);

	vr::VRInput()->GetActionHandle("/actions/demo/out/Haptic_Right", &m_rHand[Right].m_actionHaptic);
	vr::VRInput()->GetInputSourceHandle("/user/hand/right", &m_rHand[Right].m_source);
	vr::VRInput()->GetActionHandle("/actions/demo/in/Hand_Right", &m_rHand[Right].m_actionPose);



	//getting the render size of headset's eyes.
	this->pHMD->GetRecommendedRenderTargetSize(&renderWidth, &renderHeight);

	//Setup eye matrices.
	this->ProjectionMatrixLeftEye = getEyeProjectionMatrix(vr::Eye_Left);
	this->ProjectionMatrixRightEye = getEyeProjectionMatrix(vr::Eye_Right);

	this->ViewMatrixLeftEye = getEyeViewMatrix(vr::Eye_Left);
	this->ViewMatrixRightEye = getEyeViewMatrix(vr::Eye_Right);

	//sets the default position of the headset to 0,0,0.
	this->mat4HMDPose = glm::mat4(1.0f);

	return 0;
}

void VRLoader::refresh() {
//-----------------------------------------------------------------------------
// Purpose: Refreshes the HMD pose. Should be called every frame.
//
// Returns: N/A
//-----------------------------------------------------------------------------
	updateHMDPose();

}

void VRLoader::render(vr::Texture_t Left, vr::Texture_t Right) {
	//puts the textures into the eyes
	vr::VRCompositor()->Submit(vr::Eye_Left, &Left);
	vr::VRCompositor()->Submit(vr::Eye_Right, &Right);
}

glm::mat4 VRLoader::getHeadsetMatrix() {
	return this->mat4HMDPose;
}


void VRLoader::renderControllers(GLuint shader, glm::mat4 ViewMatrix) {
	glUseProgram(shader); //use the shader we passed in.
	//loop through hands
	for (EHand eHand = Left; eHand <= Right; ((int&)eHand)++)
	{
		//if controller is not to be shown, dont render it.
		if (!m_rHand[eHand].m_bShowController || !m_rHand[eHand].m_pRenderModel.isModelCompiled)
			//controllerPositions[eHand] = glm::mat4(NULL);
			continue;

		//get thhe controllers matrices, and multiply them by the view matrix to get the MVP matrix, so we can pass it to shader.
		glm::mat4 matDeviceToTracking = m_rHand[eHand].m_rmat4Pose;

		//std::cout << "Device coordinates:\n " << glm::to_string(glm::vec3(matDeviceToTracking[3])) << std::endl;
		controllerPositions[eHand] = matDeviceToTracking;

		glm::mat4 matMVP = ViewMatrix * matDeviceToTracking;

		//pass it to shader.
		glUniformMatrix4fv(glGetUniformLocation(shader, "matrix"), 1, GL_FALSE, value_ptr(matMVP));

		//render it! ^_^ (we pass the shader so we can have different shaders for different things.)
		m_rHand[eHand].m_pRenderModel.render(shader);
	}
}

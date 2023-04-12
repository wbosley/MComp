#include "VRLoader.h"

VRLoader::VRLoader() {
//-----------------------------------------------------------------------------
// Purpose: Constructor.
//-----------------------------------------------------------------------------

	this->nearClip = 0.1f;
	this->farClip = 30.0f;
}

VRLoader::~VRLoader() {
	//-----------------------------------------------------------------------------
	// Purpose: Destructor.
	//-----------------------------------------------------------------------------
}

std::string VRLoader::GetTrackedDeviceString(vr::TrackedDeviceIndex_t unDevice, vr::TrackedDeviceProperty prop, vr::TrackedPropertyError* peError = NULL)
{
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
	vr::RenderModel_t* pModel;
	vr::EVRRenderModelError error;
	while (1) {
		error = vr::VRRenderModels()->LoadRenderModel_Async(renderModelName, &pModel);
		if (error != vr::VRRenderModelError_Loading) {
			break;
		}
	}
	

	std::cout << "Triangle count: " << pModel->unTriangleCount << std::endl;

	//vr::RenderModel_Vertex_t* pVerts[pModel->unVertexCount]
	
	//Convert pModel vertices into a vector of glm vertices
	std::vector<glm::vec3> vertices;
	for (int i = 0; i < pModel->unVertexCount; i++) {
		vertices.push_back(glm::vec3(pModel->rVertexData[i].vPosition.v[0], pModel->rVertexData[i].vPosition.v[1], pModel->rVertexData[i].vPosition.v[2]));
	}

	//Convert pModel indices into a vector of unsigned int
	std::vector<unsigned int> indices;
	for (int i = 0; i < pModel->unTriangleCount * 3; i++) {
		indices.push_back(pModel->rIndexData[i]);
	}

	Model3D controller;
	controller.loadOpenVRModel(vertices, indices);


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
	m_rHand[Left].m_bShowController = true;
	m_rHand[Right].m_bShowController = true;

	vr::VREvent_t event;
	while (pHMD->PollNextEvent(&event, sizeof(event)))
	{
		ProcessVREvent(event);
	}
	vr::VRActiveActionSet_t actionSet = { 0 };
	actionSet.ulActionSet = m_actionsetDemo;
	vr::VRInput()->UpdateActionState(&actionSet, sizeof(actionSet), 1);

	for (EHand eHand = Left; eHand <= Right; ((int&)eHand)++)
	{
		vr::InputPoseActionData_t poseData;

		if (vr::VRInput()->GetPoseActionDataForNextFrame(m_rHand[eHand].m_actionPose, vr::TrackingUniverseStanding, &poseData, sizeof(poseData), vr::k_ulInvalidInputValueHandle) != vr::VRInputError_None
			|| !poseData.bActive || !poseData.pose.bPoseIsValid)
		{
			m_rHand[eHand].m_bShowController = false;
		}
		else
		{
			m_rHand[eHand].m_rmat4Pose = convertSteamVRMatrix(poseData.pose.mDeviceToAbsoluteTracking);

			vr::InputOriginInfo_t originInfo;
			if (vr::VRInput()->GetOriginTrackedDeviceInfo(poseData.activeOrigin, &originInfo, sizeof(originInfo)) == vr::VRInputError_None
				&& originInfo.trackedDeviceIndex != vr::k_unTrackedDeviceIndexInvalid)
			{
				std::string sRenderModelName = GetTrackedDeviceString(originInfo.trackedDeviceIndex, vr::Prop_RenderModelName_String);
				if (sRenderModelName != m_rHand[eHand].m_sRenderModelName)
				{
					std::cout << sRenderModelName.c_str() << std::endl;
					m_rHand[eHand].m_pRenderModel = FindOrLoadRenderModel(sRenderModelName.c_str());
					m_rHand[eHand].m_sRenderModelName = sRenderModelName;
				}
			}
		}
	}
}

int VRLoader::initVR() {
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
	pHMD = vr::VR_Init(&evrError, vr::VRApplication_Scene);

	if (evrError != vr::VRInitError_None) {
		std::cout << "Could not load runtime." << std::endl;
		return -1;
	}

	vr::EVRInitError cError = vr::VRInitError_None;

	if (!vr::VRCompositor()) {
		std::cout << "Could not load Compositor." << std::endl;
		return -1;
	}

	vr::EVRInputError error = vr::VRInputError_None;

	error = vr::VRInput()->SetActionManifestPath("C:/Users/Eleva/Documents/GitHub/MComp/DockItUI/src/hellovr_actions.json");

	std::cout << "error: " << error << std::endl;

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



	//I assume this is getting the render size of headset's eyes.
	this->pHMD->GetRecommendedRenderTargetSize(&renderWidth, &renderHeight);

	//Create frame buffer for left and right eye using the render size.
	//createFrameBuffer(renderWidth, renderHeight, LeftEyeFrameBuffer);
	//createFrameBuffer(renderWidth, renderHeight, RightEyeFrameBuffer);

	//Setup eye matrices.

	this->ProjectionMatrixLeftEye = getEyeProjectionMatrix(vr::Eye_Left);
	this->ProjectionMatrixRightEye = getEyeProjectionMatrix(vr::Eye_Right);

	this->ViewMatrixLeftEye = getEyeViewMatrix(vr::Eye_Left);
	this->ViewMatrixRightEye = getEyeViewMatrix(vr::Eye_Right);

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
	vr::VRCompositor()->Submit(vr::Eye_Left, &Left);
	vr::VRCompositor()->Submit(vr::Eye_Right, &Right);
}

glm::mat4 VRLoader::getHeadsetMatrix() {
	//return this->mat4HMDPose;
	return this->mat4DevicePose[1];
}

Model3D* VRLoader::getControllerModel(EHand eHand) {
	return &m_rHand[eHand].m_pRenderModel;
}

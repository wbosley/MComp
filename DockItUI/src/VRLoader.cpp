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

	mat4DevicePose[vr::k_unTrackedDeviceIndex_Hmd] = convertSteamVRMatrix(trackedDevicePose[vr::k_unTrackedDeviceIndex_Hmd].mDeviceToAbsoluteTracking);
	if (trackedDevicePose[vr::k_unTrackedDeviceIndex_Hmd].bPoseIsValid) {
		mat4HMDPose = mat4DevicePose[vr::k_unTrackedDeviceIndex_Hmd];
		mat4HMDPose = inverse(mat4HMDPose);
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

	error = vr::VRInput()->SetActionManifestPath("src/hellovr_bindings_generic.json");


	//I assume this is getting the render size of headset's eyes.
	pHMD->GetRecommendedRenderTargetSize(&renderWidth, &renderHeight);

	//Create frame buffer for left and right eye using the render size.
	//createFrameBuffer(renderWidth, renderHeight, LeftEyeFrameBuffer);
	//createFrameBuffer(renderWidth, renderHeight, RightEyeFrameBuffer);

	//Setup eye matrices.

	ProjectionMatrixLeftEye = getEyeProjectionMatrix(vr::Eye_Left);
	ProjectionMatrixRightEye = getEyeProjectionMatrix(vr::Eye_Right);

	ViewMatrixLeftEye = getEyeViewMatrix(vr::Eye_Left);
	ViewMatrixRightEye = getEyeViewMatrix(vr::Eye_Right);

	mat4HMDPose = glm::mat4(1.0f);

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

void::VRLoader::render(vr::Texture_t Left, vr::Texture_t Right) {
	vr::VRCompositor()->Submit(vr::Eye_Left, &Left);
	vr::VRCompositor()->Submit(vr::Eye_Right, &Right);
}
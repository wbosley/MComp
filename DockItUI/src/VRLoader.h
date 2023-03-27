#pragma once
#include <openvr.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <GL/glew.h>
class VRLoader
{
	public:
		VRLoader();
		~VRLoader();
		int initVR();
		void shutdownVR();
		void updateVR();
		glm::mat4 getEyeViewProjectionMatrix(vr::Hmd_Eye eye);
		void refresh();
		void render(vr::Texture_t Left, vr::Texture_t Right);
		GLuint renderWidth;
		GLuint renderHeight;
	private:
		vr::TrackedDevicePose_t trackedDevicePose[vr::k_unMaxTrackedDeviceCount];
		glm::mat4 mat4DevicePose[vr::k_unMaxTrackedDeviceCount];
		glm::mat4 mat4HMDPose;
		glm::mat4 ProjectionMatrixLeftEye;
		glm::mat4 ProjectionMatrixRightEye;
		glm::mat4 ViewMatrixLeftEye;
		glm::mat4 ViewMatrixRightEye;
		float nearClip;
		float farClip;
		vr::IVRSystem* pHMD;
		void updateHMDPose();
		glm::mat4 getEyeViewMatrix(vr::Hmd_Eye eye);
		glm::mat4 getEyeProjectionMatrix(vr::Hmd_Eye eye);
		glm::mat4 convertSteamVRMatrix(const vr::HmdMatrix34_t& pose);
};


#pragma once
#include <openvr.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <GL/glew.h>
#include "Model3D.h"

class VRLoader
{

	public:
		VRLoader();
		~VRLoader();
		enum EHand
		{
			Left = 0,
			Right = 1,
		};
		int initVR();
		void shutdownVR();
		void updateVR();
		glm::mat4 getEyeViewProjectionMatrix(vr::Hmd_Eye eye);
		void refresh();
		void render(vr::Texture_t Left, vr::Texture_t Right);
		void handleInput();
		GLuint renderWidth;
		GLuint renderHeight;
		glm::mat4 getHeadsetMatrix();
		Model3D* getControllerModel(EHand eHand);
	private:
		vr::TrackedDevicePose_t trackedDevicePose[vr::k_unMaxTrackedDeviceCount];
		glm::mat4 mat4DevicePose[vr::k_unMaxTrackedDeviceCount];
		glm::mat4 mat4HMDPose;
		glm::mat4 ProjectionMatrixLeftEye;
		glm::mat4 ProjectionMatrixRightEye;
		glm::mat4 ViewMatrixLeftEye;
		glm::mat4 ViewMatrixRightEye;
		char m_rDevClassChar[vr::k_unMaxTrackedDeviceCount];
		float nearClip;
		float farClip;
		vr::IVRSystem* pHMD;
		void updateHMDPose();
		void ProcessVREvent(const vr::VREvent_t& event);
		glm::mat4 getEyeViewMatrix(vr::Hmd_Eye eye);
		glm::mat4 getEyeProjectionMatrix(vr::Hmd_Eye eye);
		glm::mat4 convertSteamVRMatrix(const vr::HmdMatrix34_t& pose);
		std::string GetTrackedDeviceString(vr::TrackedDeviceIndex_t unDevice, vr::TrackedDeviceProperty prop, vr::TrackedPropertyError* peError);
		Model3D FindOrLoadRenderModel(const char* renderModelName);

		vr::VRActionSetHandle_t m_actionsetDemo = vr::k_ulInvalidActionSetHandle;
		vr::VRActionHandle_t m_actionHideCubes = vr::k_ulInvalidActionHandle;
		vr::VRActionHandle_t m_actionHideThisController = vr::k_ulInvalidActionHandle;
		vr::VRActionHandle_t m_actionTriggerHaptic = vr::k_ulInvalidActionHandle;
		vr::VRActionHandle_t m_actionAnalongInput = vr::k_ulInvalidActionHandle;
		struct ControllerInfo_t
		{
			vr::VRInputValueHandle_t m_source = vr::k_ulInvalidInputValueHandle;
			vr::VRActionHandle_t m_actionPose = vr::k_ulInvalidActionHandle;
			vr::VRActionHandle_t m_actionHaptic = vr::k_ulInvalidActionHandle;
			glm::mat4 m_rmat4Pose;
			Model3D m_pRenderModel;
			std::string m_sRenderModelName;
			bool m_bShowController;
		};


		ControllerInfo_t m_rHand[2];
		
};


#pragma once
#include <windows.h>
#include <openvr.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <GL/glew.h>
#include "Model3D.h"
#include <string>
#include "GUILoader.h"
#include <algorithm>

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
		glm::mat4 getControllerMatrix(EHand hand);
		void refresh();
		void render(vr::Texture_t Left, vr::Texture_t Right);
		void renderControllers(GLuint shader, glm::mat4);
		void handleInput();
		void parseGuiLoader(GUILoader* guiLoader);
		GLuint renderWidth;
		GLuint renderHeight;
		glm::mat4 getHeadsetMatrix();
		glm::mat4 controllerPositions[2];
		struct ControllerInfo_t
		{
			vr::VRInputValueHandle_t m_source = vr::k_ulInvalidInputValueHandle;
			vr::VRActionHandle_t m_actionPose = vr::k_ulInvalidActionHandle;
			vr::VRActionHandle_t m_actionHaptic = vr::k_ulInvalidActionHandle;
			glm::mat4 m_rmat4Pose;
			glm::vec3 headsetDisplacement;
			Model3D m_pRenderModel;
			std::string m_sRenderModelName;
			bool m_bShowController;
		};
		ControllerInfo_t m_rHand[2];
		bool interactButton = false;
		bool hapticButton = false;
		glm::vec2 analogInput = glm::vec2(0.0f,0.0f);
	private:
		vr::TrackedDevicePose_t trackedDevicePose[vr::k_unMaxTrackedDeviceCount];
		glm::mat4 mat4DevicePose[vr::k_unMaxTrackedDeviceCount];
		glm::mat4 mat4HMDPose;
		glm::mat4 ProjectionMatrixLeftEye;
		glm::mat4 ProjectionMatrixRightEye;
		glm::mat4 ViewMatrixLeftEye;
		glm::mat4 ViewMatrixRightEye;
		GUILoader* guiLoader;
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
		bool GetDigitalActionState(vr::VRActionHandle_t action, vr::VRInputValueHandle_t* pDevicePath);
		vr::VRActionSetHandle_t m_actionsetDemo = vr::k_ulInvalidActionSetHandle;
		vr::VRActionHandle_t m_actionsInteract = vr::k_ulInvalidActionHandle;
		vr::VRActionHandle_t m_actionTriggerHaptic = vr::k_ulInvalidActionHandle;
		vr::VRActionHandle_t m_actionAnalongInput = vr::k_ulInvalidActionHandle;
};


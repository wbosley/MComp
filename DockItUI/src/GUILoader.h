#pragma once
//include glew
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "ImGui3D.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


class GUILoader
{
	enum CAMERA_MODE
	{
		VR_VIEW, MONITOR_VIEW
	};
	public:

		GUILoader();
		~GUILoader();
		int initGLFWGui(GLFWwindow* window);
		int initVRGui();
		void renderWindowGui();
		void renderVRGui(glm::mat4 ViewMatrix);
		void vrWindowInfo(int index);
		CAMERA_MODE getCameraMode();
		std::vector<ImGui3D>* getVRWindows();
		bool reverseProtein = false;
		bool pickUpWindow = false;
		private:
		GLFWwindow* window;
		ImGuiIO* io;
		CAMERA_MODE camera_mode = MONITOR_VIEW;
		ImGuiContext* context_glfw;
		std::vector<ImGui3D> vr_windows;
		float f = 0.0f;
};


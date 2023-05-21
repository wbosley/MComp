#pragma once
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
		void renderVRGui(glm::mat4 matrix);
		CAMERA_MODE getCameraMode();
	private:
		GLFWwindow* window;
		ImGuiIO* io;
		CAMERA_MODE camera_mode = VR_VIEW;
		ImGuiContext* context_glfw;
		ImGuiContext** context_vr;


};


#pragma once
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "FrameBuffer.h"
#include "Model3D.h"
#include <glm/gtx/string_cast.hpp>

class ImGui3D
{
	private:
		ImGuiContext* context;
		int width;
		int height;
		ImVec2 mousePos;
		ImGui3D *parent = nullptr;

		
	public:
		ImGui3D(ImGuiContext* context);
		ImGui3D(ImGuiContext* context, int width, int height);
		~ImGui3D();
		void start();
		void render(GLuint shader);
		void setMousePosition(ImVec2 position);
		void setClicked(bool clicked);
		void setBeingMoved(bool moved, int hand);
		void makeChildOfWindow(ImGui3D* parent);
		FrameBuffer frameBuffer;
		Model3D* quad;
		int info;
		bool showWindow = true;
		bool attached = false;
		int hand;

};


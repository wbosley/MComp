#pragma once
#include "Quad3D.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "FrameBuffer.h"

class ImGui3D
{
	private:
		ImGuiContext* context;
		int width;
		int height;
		ImVec2 mousePos;
		
	public:
		ImGui3D(ImGuiContext* context);
		~ImGui3D();
		void start();
		void render();
		void setMousePosition(ImVec2 position);
		void setClicked(bool clicked);
		FrameBuffer frameBuffer;
		Quad3D* quad;

};


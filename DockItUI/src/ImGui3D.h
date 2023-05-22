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
		Quad3D* quad;
	public:
		ImGui3D(ImGuiContext* context, int width, int height);
		~ImGui3D();
		void start();
		void render();
		FrameBuffer frameBuffer;

};


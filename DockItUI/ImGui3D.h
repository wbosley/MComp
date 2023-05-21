#pragma once
class ImGui3D
{
	private:
		ImGui3D();
		~ImGui3D();
	public:
		int init();
		void compile();
		void render();
};


#include "GUILoader.h"


GUILoader::GUILoader()
{
}

GUILoader::~GUILoader() {
}

int GUILoader::init(GLFWwindow* window) {
	this->window = window;
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	io = &ImGui::GetIO();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 460");

	return 0;
}

void GUILoader::renderWindowGui() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("FPS Counter");
	ImGui::Text("FPS: %.1f", io->Framerate);
	ImGui::End();
	ImGui::Render();

	//Render function for rendering ImGui.
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUILoader::renderVRGui(glm::mat4 matrix) {
	ImGui_ImplOpenGL3_NewFrame();
	
	ImGui::NewFrame();
	ImGui::Begin("FPS Counter");
	//ImGui::Text("FPS: %.1f", io->Framerate);
	ImGui::End();
	ImGui::Render();

	ImGui_ImplOpenGL3VR_RenderDrawData(ImGui::GetDrawData(), matrix);
}
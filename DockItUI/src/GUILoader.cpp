#include "GUILoader.h"


GUILoader::GUILoader()
{
	IMGUI_CHECKVERSION();
}

GUILoader::~GUILoader() {
}

int GUILoader::initGLFWGui(GLFWwindow* window) {
	this->window = window;
	this->context_glfw = ImGui::CreateContext();
	ImGui::SetCurrentContext(this->context_glfw);
	io = &ImGui::GetIO();
	ImGui::StyleColorsDark();
	ImGui_ImplOpenGL3_Init("#version 460");
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	return 0;
}

int GUILoader::initVRGui() {
	ImGui3D vrWindow = ImGui3D(ImGui::CreateContext(), 200, 200);
	vr_windows.push_back(vrWindow);
	vrWindow = ImGui3D(ImGui::CreateContext(), 200, 200);
	vr_windows.push_back(vrWindow);
	return 0;
}

GUILoader::CAMERA_MODE GUILoader::getCameraMode() {
	return camera_mode;
}

void GUILoader::renderWindowGui() {
	ImGui::SetCurrentContext(this->context_glfw);
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowCollapsed(false);
	ImGui::SetNextWindowSize(ImVec2(200, 200));
	//ImGui::SetNextWindowSize(ImVec2(io->DisplaySize.x/4, io->DisplaySize.y/4));
	//ImGui::SetNextWindowSizeConstraints(ImVec2(200, 200), ImVec2(300, 400));

	static bool p_open = true;
	static bool no_titlebar = false;
	static bool no_scrollbar = true;
	static bool no_menu = true;
	static bool no_move = true;
	static bool no_resize = true;
	static bool no_collapse = true;
	static bool no_close = true;
	static bool no_nav = false;
	static bool no_background = false;
	static bool no_bring_to_front = true;
	static bool unsaved_document = false;
	

	ImGuiWindowFlags window_flags = 0;
	if (no_titlebar)        window_flags |= ImGuiWindowFlags_NoTitleBar;
	if (no_scrollbar)       window_flags |= ImGuiWindowFlags_NoScrollbar;
	if (!no_menu)           window_flags |= ImGuiWindowFlags_MenuBar;
	if (no_move)            window_flags |= ImGuiWindowFlags_NoMove;
	if (no_resize)          window_flags |= ImGuiWindowFlags_NoResize;
	if (no_collapse)        window_flags |= ImGuiWindowFlags_NoCollapse;
	if (no_nav)             window_flags |= ImGuiWindowFlags_NoNav;
	if (no_background)      window_flags |= ImGuiWindowFlags_NoBackground;
	if (no_bring_to_front)  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
	if (unsaved_document)   window_flags |= ImGuiWindowFlags_UnsavedDocument;
	if (no_close)           p_open = NULL; // Don't pass our bool* to Begin


	ImGui::Begin("DockIt User Interface", NULL, window_flags);
	ImGui::Text("FPS: %.1f", io->Framerate);
	ImGui::Text("Camera Mode: ");
	if (ImGui::Button("VR Camera")) {
		camera_mode = VR_VIEW;
	}
	if (ImGui::Button("Keyboard controlled Camera")) {
		camera_mode = MONITOR_VIEW;
	}
	ImGui::End();
	ImGui::Render();

	//Render function for rendering ImGui.
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUILoader::renderVRGui(std::vector<glm::mat4> *matrices) {
	GLuint shader;
	glGetIntegerv(GL_CURRENT_PROGRAM, reinterpret_cast<GLint*>(&shader));
	glUseProgram(shader);
	if (matrices->size() != vr_windows.size()) {
		std::cout << "ERROR: Matrices and VR windows are not the same size!" << std::endl;
		return;
	}

	for (int i = 0; i < matrices->size(); i++) {
		glUniformMatrix4fv(glGetUniformLocation(shader, "matrix"), 1, GL_FALSE, value_ptr(matrices->at(i)));
		vr_windows[i].start();
		vrWindowInfo(i);
		vr_windows[i].render();
	}
		
	//int index = 0;


	//glUniformMatrix4fv(glGetUniformLocation(shader, "matrix"), 1, GL_FALSE, value_ptr(matrices->at(index)));
	//index++;
	//vr_windows[0].start();
	//ImGui::Begin("DockIt User Interface", NULL);
	//ImGui::Text("THIS IS A TEST WINDOW :)");
	//ImGui::End();
	//vr_windows[0].render();

	//glUniformMatrix4fv(glGetUniformLocation(shader, "matrix"), 1, GL_FALSE, value_ptr(matrices->at(index)));
	//index++;
	//vr_windows[1].start();
	//vr_windows[1].render();
}

void GUILoader::vrWindowInfo(int index) {
	switch (index) {
	case 0:
		ImGui::Begin("DockIt User Interface", NULL);
		ImGui::Text("THIS IS A TEST WINDOW :)");
		ImGui::Button("Test Button");
		ImGui::End();
		break;
	case 1:
		ImGui::Begin("DockIt User Interface", NULL);
		ImGui::Text("THIS IS A TEST WINDOW 2 :)");
		ImGui::Button("Test Button");
		ImGui::End();
		break;
	default:
		ImGui::Begin("Something went wrong :L", NULL);
		ImGui::Text("Indexing error lol");
		ImGui::End();
		break;
	}

}

std::vector<ImGui3D>* GUILoader::getVRWindows() {
	return &vr_windows;
}
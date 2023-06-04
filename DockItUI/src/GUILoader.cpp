#include "GUILoader.h"


GUILoader::GUILoader()
{
	IMGUI_CHECKVERSION();
	colour = { 1.0f, 0.0f, 1.0f, 0.5f };
}

GUILoader::~GUILoader() {
}

int GUILoader::initGLFWGui(GLFWwindow* window) {
	//Initialise the GLFW ImGUI context
	this->window = window;
	this->context_glfw = ImGui::CreateContext();
	ImGui::SetCurrentContext(this->context_glfw);
	io = &ImGui::GetIO();
	ImGui::StyleColorsDark();
	ImGui_ImplOpenGL3_Init("#version 460");
	//Initialise the GLFW ImGUI implementation for OpenGL
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	return 0;
}

int GUILoader::initVRGui() {
	//Create all our VR windows
	int VR_WINDOW_COUNT = 5;

	for (int i = 0; i < VR_WINDOW_COUNT; i++) {

		if (i != 4) {
			vr_windows.push_back(new ImGui3D(ImGui::CreateContext()));
			vr_windows.at(i)->info = i;
			//info is used to determine what the window should be populated with 
			//in the VrWindowInfo function
		}
		else {
			vr_windows.push_back(new ImGui3D(ImGui::CreateContext(), 400, 120));
			vr_windows.at(i)->info = i;
			//special cases handled here and below
		}

		if (i == 1) {
			vr_windows.at(i)->showWindow = false;
		}
		if (i == 2) {
			vr_windows.at(i)->makeChildOfWindow(vr_windows.at(0));
		}
	}
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

void GUILoader::renderVRGui(glm::mat4 ViewMatrix) {
	//Render the VR windows
	GLuint shader;
	glGetIntegerv(GL_CURRENT_PROGRAM, reinterpret_cast<GLint*>(&shader));
	glUseProgram(shader);
	//Loop through all the VR windows and render them
	for (int i = 0; i < vr_windows.size(); i++) {
		if (!vr_windows[i]->showWindow) {
			continue;
		}
		glUniformMatrix4fv(glGetUniformLocation(shader, "matrix"), 1, GL_FALSE, value_ptr(ViewMatrix * vr_windows[i]->quad->ModelMatrix));
		vr_windows[i]->start();
		vrWindowInfo(vr_windows[i]->info);
		vr_windows[i]->render(shader);
	}
}

void GUILoader::vrWindowInfo(int index) {
	//Function for populating the VR windows with widgets.
	static bool p_open = true;
	bool no_titlebar = false;
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



	switch (index) {
		case 0:{
			
			ImGui::Begin("DockIt VR Multi-Window", NULL, window_flags);
			ImGui::SetWindowFontScale(2.5f);
			ImGui::Text("This window can show\npreviews of the next\nwindow when hovering\nover buttons.");
			
			
			if (ImGui::Button("Movement settings")) {
				vr_windows.at(0)->info = 2;
				vr_windows.at(2)->info = 0;
			}
			if (ImGui::IsItemHovered() && vr_windows.at(2)->info != 0) {
				vr_windows.at(2)->info = 2;
			}
			ImGui::Text("\n");
			if (ImGui::Button("Colour settings")) {
				vr_windows.at(0)->info = 3;
				vr_windows.at(2)->info = 0;
			}
			if (ImGui::IsItemHovered() && vr_windows.at(2)->info != 0) {
				vr_windows.at(2)->info = 3;
			}
			//ImGui::Text("Mouse clicked: %s", ImGui::IsMouseDown(ImGuiMouseButton_Left) ? "Yes" : "No");
			ImGui::End();
			break;
		}
		case 1: {
			ImGui::Begin("Interaction Window", NULL, window_flags);
			ImGui::SetWindowFontScale(2.5f);
			ImGui::Text("This window will show\ninformation about what\nyou are currently\nselecting.");
			ImGui::Text(("Atom: " + ASW_atomName).c_str());
			ImGui::End();
			break;
		}
		case 2: {
			ImGui::Begin("Movement settings", NULL, window_flags);
			ImGui::SetWindowFontScale(2.5f);
			ImGui::Text("Movement settings:\n");
			if (ImGui::Button("Spin protein")) {
				reverseProtein = !reverseProtein;
			}
			ImGui::Text("\n\n");
			if (ImGui::Button("Back")) {
				vr_windows.at(0)->info = 0;
				vr_windows.at(2)->info = 2;
			}
			ImGui::End();
			break;
		}
		case 3: {
			ImGui::Begin("Protein Colour settings", NULL, window_flags);
			ImGui::Text("Colour settings:\n");
			ImGui::ColorPicker4("ColourPicker", (float*)&colour);
			if (ImGui::Button("Back")) {
				vr_windows.at(0)->info = 0;
				vr_windows.at(2)->info = 3;
			}
			ImGui::End();
			break;
		}
		case 4: {
			ImGui::Begin("Welcome to DockItUI!", NULL, window_flags);
			ImGui::SetWindowFontScale(2.5f);
			ImGui::Text("Welcome to the\nDockItUI interface\nprototyping testing\narea.");
			ImGui::End();
			break;
		}
		case 5: {
			no_titlebar = true;
			ImGuiWindowFlags window_flags2 = 0;
			if (no_titlebar)        window_flags2 |= ImGuiWindowFlags_NoTitleBar;
			if (no_scrollbar)       window_flags2 |= ImGuiWindowFlags_NoScrollbar;
			if (!no_menu)           window_flags2 |= ImGuiWindowFlags_MenuBar;
			if (no_move)            window_flags2 |= ImGuiWindowFlags_NoMove;
			if (no_resize)          window_flags2 |= ImGuiWindowFlags_NoResize;
			if (no_collapse)        window_flags2 |= ImGuiWindowFlags_NoCollapse;
			if (no_nav)             window_flags2 |= ImGuiWindowFlags_NoNav;
			if (no_background)      window_flags2 |= ImGuiWindowFlags_NoBackground;
			if (no_bring_to_front)  window_flags2 |= ImGuiWindowFlags_NoBringToFrontOnFocus;
			if (unsaved_document)   window_flags2|= ImGuiWindowFlags_UnsavedDocument;
			if (no_close)           p_open = NULL;
			ImGui::Begin("Taskbar information", NULL, window_flags2);
			ImGui::Text("Max Atom Penetration: 0.00A    |   L: gmx_1GRU_GroES");
			ImGui::Text("Cutoff: 8.00A");
			ImGui::Text("R: gmx_1GRU_GroEL");
			ImGui::End();
			break;
		}
		default: {
			ImGui::Begin("Something went wrong :L", NULL);
			ImGui::Text("Indexing error lol");
			ImGui::End();
			break;
		}
	}

}

std::vector<ImGui3D*>* GUILoader::getVRWindows() {
	return &vr_windows;
}

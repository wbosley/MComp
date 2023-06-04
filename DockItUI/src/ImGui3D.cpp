#include "ImGui3D.h"


ImGui3D::ImGui3D(ImGuiContext* context) {
	//Create the ImGui3D object
	this->context = context;
	//Set the width and height of the window texture resolution
	this->width = 400;
	this->height = 400;
	this->frameBuffer.createFrameBuffer(this->width, this->height);
	//Create a quad to render the texture to
	this->quad = new Model3D();
	this->quad->createQuad(this->frameBuffer.m_nRenderTextureId, true);
	this->quad->compileModel();
	ImGui::SetCurrentContext(this->context);
	ImGui::StyleColorsDark();
	// Initialise the OpenGL3 implementation of ImGui
	ImGui_ImplOpenGL3_Init("#version 460");
	//std::cout << "ImGui3D created" << this << std::endl;
}
ImGui3D::ImGui3D(ImGuiContext* context, int width, int height) {
	this->context = context;
	this->width = width;
	this->height = height;
	this->frameBuffer.createFrameBuffer(this->width, this->height);
	this->quad = new Model3D();
	this->quad->createQuad(this->frameBuffer.m_nRenderTextureId, true);
	this->quad->compileModel();
	ImGui::SetCurrentContext(this->context);
	ImGui::StyleColorsDark();
	ImGui_ImplOpenGL3_Init("#version 460");
	//std::cout << "ImGui3D created" << this << std::endl;
}
ImGui3D::~ImGui3D() {
}

void ImGui3D::start() {
	//This function is called for every window every frame and is used to set up the ImGui context
	ImGui::SetCurrentContext(this->context);
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplVR_NewFrame(this->width, this->height, this->mousePos);
	ImGui::NewFrame();
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowCollapsed(false);
	ImGui::SetNextWindowSize(ImVec2(this->width, this->height));

	//std::cout << this->parent << std::endl;

	if (this->parent != nullptr) {
		//std::cout << this->parent->quad->ModelMatrix[3][1] << std::endl;
		if (this->parent->info == 0) {
			
			this->quad->ModelMatrix = glm::rotate(glm::translate(this->parent->quad->ModelMatrix, glm::vec3(2.3, 0, -0.3)), 6.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		}
		else {
			this->quad->ModelMatrix = glm::rotate(glm::translate(this->parent->quad->ModelMatrix, glm::vec3(-2.3, 0, -0.3)), (2.0f * 3.14159265f) - 6.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		}
		
	}

}
void ImGui3D::render(GLuint shader) {
	//This function is called for every window every frame and is used to render the ImGui context to the frame buffer.
	GLint last_viewport[4]; glGetIntegerv(GL_VIEWPORT, last_viewport);
	GLint last_framebuffer; glGetIntegerv(GL_FRAMEBUFFER_BINDING, &last_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, this->frameBuffer.m_nRenderFramebufferId);
	ImGuiIO& io = ImGui::GetIO();
	ImGui::Render();
	glViewport(0, 0, this->width, this->height);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glBindFramebuffer(GL_FRAMEBUFFER, last_framebuffer);
	glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2], (GLsizei)last_viewport[3]);
	this->quad->render(shader);
}

void ImGui3D::setMousePosition(ImVec2 position) {
	//Set the mouse position for the ImGui context
	if (this->parent == nullptr) {	//this->mousePos = position;
		ImGui::SetCurrentContext(this->context);
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = position;
	}
}

void ImGui3D::setClicked(bool clicked) {
	//Set the mouse to be clicked for the ImGui context
	if (this->parent == nullptr) {
		ImGui::SetCurrentContext(this->context);
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[0] = clicked;
	}
}

void ImGui3D::setBeingMoved(bool moved, int hand) {
	//Set the window as currently being moved by the user
	if (this->parent == nullptr) {
		this->attached = moved;
		this->hand = hand;
	}
}

void ImGui3D::makeChildOfWindow(ImGui3D* parent) {
	//Make this window a child of another window
	this->parent = parent;
}

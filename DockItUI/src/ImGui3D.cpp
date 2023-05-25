#include "ImGui3D.h"


ImGui3D::ImGui3D(ImGuiContext* context) {
	this->context = context;
	this->width = 200;
	this->height = 200;
	//this->quad = new Quad3D();
	this->frameBuffer.createFrameBuffer(width, height);
	//this->quad->loadTexture(frameBuffer.m_nRenderTextureId, width, height);
	this->quad = new Model3D();
	this->quad->createQuad(this->frameBuffer.m_nRenderTextureId);
	std::cout << "texture id: " << this->frameBuffer.m_nRenderTextureId << "\n";
	this->quad->compileModel();
	ImGui::SetCurrentContext(this->context);
	ImGui::StyleColorsDark();
	ImGui_ImplOpenGL3_Init("#version 460");
}
ImGui3D::~ImGui3D() {
}

void ImGui3D::start() {
	
	ImGui::SetCurrentContext(this->context);
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplVR_NewFrame(this->width, this->height, this->mousePos);
	ImGui::NewFrame();
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowCollapsed(false);
	ImGui::SetNextWindowSize(ImVec2(this->width, this->height));
}
void ImGui3D::render(GLuint shader) {
	GLint last_viewport[4]; glGetIntegerv(GL_VIEWPORT, last_viewport);
	GLint last_framebuffer; glGetIntegerv(GL_FRAMEBUFFER_BINDING, &last_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, this->frameBuffer.m_nRenderFramebufferId);
	ImGuiIO& io = ImGui::GetIO();
	ImGui::Render();
	glViewport(0, 0, this->width, this->height);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glBindFramebuffer(GL_FRAMEBUFFER, last_framebuffer);
	glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2], (GLsizei)last_viewport[3]);
	//this->quad->loadTexture(this->frameBuffer.m_nRenderTextureId, this->width, this->height);
	this->quad->render(shader);
	//this->quad->render();
}

void ImGui3D::setMousePosition(ImVec2 position) {
	//this->mousePos = position;
	ImGui::SetCurrentContext(this->context);
	ImGuiIO& io = ImGui::GetIO();
	io.MousePos = position;
}

void ImGui3D::setClicked(bool clicked) {
	ImGui::SetCurrentContext(this->context);
	ImGuiIO& io = ImGui::GetIO();
	io.MouseDown[0] = clicked;
}

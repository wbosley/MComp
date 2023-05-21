#include "ImGui3D.h"


ImGui3D::ImGui3D(ImGuiContext* context, int width, int height, GLuint shader) {
	this->context = context;
	this->width = width;
	this->height = height;
	this->shader = shader;
	this->quad = new Quad3D();
	this->quad->compileQuad();
	this->frameBuffer.createFrameBuffer(width, height);
	this->quad->loadTexture(frameBuffer.m_nRenderTextureId, width, height);
	ImGui::SetCurrentContext(this->context);
	ImGui::StyleColorsDark();
	ImGui_ImplOpenGL3_Init("#version 460");
}
ImGui3D::~ImGui3D() {
}

void ImGui3D::start() {
	ImGui::SetCurrentContext(this->context);
	ImGui_ImplVR_NewFrame(this->width, this->height);
	ImGui::NewFrame();
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowCollapsed(false);
	ImGui::SetNextWindowSize(ImVec2(this->width, this->height));
}
void ImGui3D::render() {
	GLint last_viewport[4]; glGetIntegerv(GL_VIEWPORT, last_viewport);
	GLint last_framebuffer; glGetIntegerv(GL_FRAMEBUFFER_BINDING, &last_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, this->frameBuffer.m_nRenderFramebufferId);
	ImGui::Render();
	glViewport(0, 0, this->width, this->height);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glBindFramebuffer(GL_FRAMEBUFFER, last_framebuffer);
	glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2], (GLsizei)last_viewport[3]);
	this->quad->loadTexture(this->frameBuffer.m_nRenderTextureId, this->width, this->height);
	this->quad->render(this->shader);
	//this->quad->render();
}

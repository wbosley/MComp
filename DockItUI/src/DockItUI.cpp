// DockItUI.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <openvr.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "OBJLoader.h"
#include "Shader.h"
#include "Model3D.h"
#include "Camera.h"
#include <math.h>
#include "VRLoader.h"
#include "ProteinLoader.h"
#include "Sphere.h"
#include "Protein3D.h"

#define FORWARD 0
#define BACKWARD 1
#define LEFT 2
#define RIGHT 3
#define UP 0
#define DOWN 1

GLFWwindow* window;
GLenum err;
int screenWidth = 800;
int screenHeight = 600;
float nearClip = 0.1f;
float farClip = 30.0f;
GLuint renderWidth;
GLuint renderHeight;
glm::mat4 ProjectionMatrix;
glm::mat4 ModelViewMatrix;
glm::mat4 ViewMatrix;
glm::mat4 ModelMatrix;
Shader myShader;
vr::IVRSystem* pHMD;
OBJLoader objLoader;
ProteinLoader proteinLoader;
Protein3D firstProtein;
Model3D firstModel;
Sphere firstSphere;
Camera camera;
struct FrameBuffer {
	GLuint m_nDepthBufferId;
	GLuint m_nRenderTextureId;
	GLuint m_nRenderFramebufferId;
};
FrameBuffer LeftEyeFrameBuffer;
FrameBuffer RightEyeFrameBuffer;
glm::mat4 ProjectionMatrixLeftEye;
glm::mat4 ProjectionMatrixRightEye;
glm::mat4 ViewMatrixLeftEye;
glm::mat4 ViewMatrixRightEye;
VRLoader vrLoader;
GLuint VAOwad;

void renderControllers() {

}

void renderAll(glm::mat4 ViewMatrix) {
//-----------------------------------------------------------------------------
// Purpose: Renders the scene with OpenGL with matrices related to a specific 
//			camera/eye's view matrix.
//
// Returns: N/A
//-----------------------------------------------------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.3f, 0.2f, 0.5f, 1.0f);

	ModelMatrix = glm::mat4(1.0f);
	ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0, 0, 7));

	glUniformMatrix4fv(glGetUniformLocation(myShader.getShaderProgram(), "matrix"), 1, GL_FALSE, value_ptr(ViewMatrix * ModelMatrix));
	//firstModel.render(myShader);
	firstProtein.render(myShader);
	renderControllers();
}

void renderCompanionWindow() {

}

void display() {
//-----------------------------------------------------------------------------
// Purpose: The main rendering loop. This function is called every frame.
//
// Returns: N/A
//-----------------------------------------------------------------------------
	glUseProgram(myShader.getShaderProgram());
	vrLoader.refresh();

	//Projection Matrix for screen.
	//glUniformMatrix4fv(glGetUniformLocation(myShader.getShaderProgram(), "ProjectionMatrix"), 1, GL_FALSE, &ProjectionMatrix[0][0]);

	glEnable(GL_MULTISAMPLE);

	// Left eye
	glBindFramebuffer(GL_FRAMEBUFFER, LeftEyeFrameBuffer.m_nRenderFramebufferId);
	glViewport(0, 0, vrLoader.renderWidth, vrLoader.renderHeight);
	renderAll(vrLoader.getEyeViewProjectionMatrix(vr::Eye_Left));
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );

	// Right eye
	glBindFramebuffer(GL_FRAMEBUFFER, RightEyeFrameBuffer.m_nRenderFramebufferId);
	glViewport(0, 0, vrLoader.renderWidth, vrLoader.renderHeight);
	renderAll(vrLoader.getEyeViewProjectionMatrix(vr::Eye_Right));
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glDisable(GL_MULTISAMPLE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, screenWidth, screenHeight);
	//so we can use the same shader for the VR headset and the companion window, we multiply the projection matrix by the view matrix here rather than in the shader.
	//renderAll(ProjectionMatrix * camera.getMatrix());

	//Code to make the companion window display view from headset location - George
	renderAll(ProjectionMatrix * vrLoader.getHeadsetMatrix());
	
	//Render to the companion window.
	renderCompanionWindow();


	// Turn framebuffer texture into an OpenVR texture.
	vr::Texture_t leftEyeTexture = { (void*)(uintptr_t)LeftEyeFrameBuffer.m_nRenderTextureId, vr::TextureType_OpenGL, vr::ColorSpace_Gamma };
	vr::Texture_t rightEyeTexture = { (void*)(uintptr_t)RightEyeFrameBuffer.m_nRenderTextureId, vr::TextureType_OpenGL, vr::ColorSpace_Gamma };

	//Render the texture to the headset.
	vrLoader.render(leftEyeTexture, rightEyeTexture);

	//Clear all buffers.
	glFlush();

}

void reshape(GLFWwindow* window, int width, int height) {
	screenWidth = width;
	screenHeight = height;
	glViewport(0, 0, screenWidth, screenHeight);
	ProjectionMatrix = glm::perspective(glm::radians(60.0f), (GLfloat)screenWidth / (GLfloat)screenHeight, 1.0f, 200.0f);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_W && action == GLFW_REPEAT || action == GLFW_PRESS) {
		camera.move(0.1, FORWARD);
	}
	if (key == GLFW_KEY_S && action == GLFW_REPEAT || action == GLFW_PRESS) {
		camera.move(0.1, BACKWARD);
	}
	if (key == GLFW_KEY_A && action == GLFW_REPEAT || action == GLFW_PRESS) {
		camera.move(0.1, LEFT);
	}
	if (key == GLFW_KEY_D && action == GLFW_REPEAT || action == GLFW_PRESS) {
		camera.move(0.1, RIGHT);
	}
	if (key == GLFW_KEY_UP && action == GLFW_REPEAT || action == GLFW_PRESS) {
		camera.rotate(0.1, UP);
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_REPEAT || action == GLFW_PRESS) {
		camera.rotate(0.1, DOWN);
	}
	if (key == GLFW_KEY_LEFT && action == GLFW_REPEAT || action == GLFW_PRESS) {
		camera.rotate(0.1, LEFT);
	}
	if (key == GLFW_KEY_RIGHT && action == GLFW_REPEAT || action == GLFW_PRESS) {
		camera.rotate(0.1, RIGHT);
	}
}

void createFrameBuffer(int width, int height, FrameBuffer& framebuffer) {
	//Frame buffers allow us to render to multiple viewports such as the left and right eye and companion window.
	//Basically we are rendering to a texture instead of the screen.
	//This allows us to render to multiple viewports without having to render the same scene multiple times.
	//This is a very useful technique for VR. - george

	//Generate a frame buffer and bind it.
	glGenFramebuffers(1, &framebuffer.m_nRenderFramebufferId);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.m_nRenderFramebufferId);

	//Generate a render buffer, bind it and specify details about the buffer.
	glGenRenderbuffers(1, &framebuffer.m_nDepthBufferId);
	glBindRenderbuffer(GL_RENDERBUFFER, framebuffer.m_nDepthBufferId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width , height);
	//Unbind the render buffer.
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	//Attach the render buffer to the frame buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, framebuffer.m_nDepthBufferId);

	//Generate a texture, bind it and specify details about the texture.
	glGenTextures(1, &framebuffer.m_nRenderTextureId);
	glBindTexture(GL_TEXTURE_2D, framebuffer.m_nRenderTextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	//Set texture parameters.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	//Attach the texture to the frame buffer.
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebuffer.m_nRenderTextureId, 0);

	//Unbind the texture.
	glBindTexture(GL_TEXTURE_2D, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Framebuffer creation successful." << std::endl;
	}
	else {
		std::cout << "Framebuffer creation FAILED." << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
}

int initModels() {
	objLoader.loadOBJ("src/models/teapot.obj");
	firstModel.loadModelFromObj(objLoader);
	firstSphere.constructGeometry(8);

	proteinLoader.loadProtein("src/proteins/Test3.pdb");
	firstProtein.loadProteinFromProteinLoader(proteinLoader);

	std::cout << "getting to end of initmodels" << std::endl;
	return 0;
}

int init() {

	//Initialise GLFW.
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	//Create window and check for errors.
	window = glfwCreateWindow(screenWidth, screenHeight, "DockItUI", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);

	//Initialise GLEW and check for errors.
	glewExperimental = GL_TRUE;
	err = glewInit();
	if (GLEW_OK != err)
	{
		std::cout << "Error: " << glewGetErrorString(err) << std::endl;
		glfwTerminate();
	}

	reshape(window, screenWidth, screenHeight);
	glfwSetFramebufferSizeCallback(window, reshape);
	glfwSetKeyCallback(window, key_callback);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);

	myShader.createShaderFromFile("src/shaders/basicvr.shader");

	if (vrLoader.initVR() != 0) {
		std::cout << "Failed to setup VR." << std::endl;
		return -1;
	}

	createFrameBuffer(vrLoader.renderWidth, vrLoader.renderHeight, LeftEyeFrameBuffer);
	createFrameBuffer(vrLoader.renderWidth, vrLoader.renderHeight, RightEyeFrameBuffer);


	if (initModels() != 0) {
		std::cout << "Failed to load models." << std::endl;
		return -1;
	}

	camera = Camera();
	camera.setPosition(glm::vec3(0.0, -2.5, -15.0));
	
	
	

}

int main()
{
	//Initialisation function.
	init();


	// Loop until the user closes the window
	while (!glfwWindowShouldClose(window))
	{
		// Main render loop.
		display();
		// Swap front and back buffers
		glfwSwapBuffers(window);

		// Poll for and process events
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
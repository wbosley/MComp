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
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "GUILoader.h"
#include "Quad3D.h"
#include "FrameBuffer.h"

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
Shader vrShader;
Shader controllerShader;
Shader quadShader;
Shader quad3DShader;
Shader proteinShader;
vr::IVRSystem* pHMD;
OBJLoader objLoader;
ProteinLoader proteinLoader;
Protein3D firstProtein;
Model3D firstModel;
Model3D secondModel;
Sphere firstSphere;
Camera camera;
FrameBuffer LeftEyeFrameBuffer;
FrameBuffer RightEyeFrameBuffer;
FrameBuffer testBuffer;
glm::mat4 ProjectionMatrixLeftEye;
glm::mat4 ProjectionMatrixRightEye;
glm::mat4 ViewMatrixLeftEye;
glm::mat4 ViewMatrixRightEye;
VRLoader vrLoader;
GUILoader guiLoader;
GLuint VAOwad;
Quad3D quad;
Quad3D quadTest;

enum CAMERA_MODE
{
	VR_VIEW, MONITOR_VIEW
};

void renderAll(glm::mat4 ViewMatrix) {
//-----------------------------------------------------------------------------
// Purpose: Renders the scene with OpenGL with matrices related to a specific 
//			camera/eye's view matrix.
//
// Returns: N/A
//-----------------------------------------------------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.3f, 0.2f, 0.5f, 1.0f);

	//glUseProgram(vrShader.getShaderProgram());
	//ModelMatrix = glm::mat4(1.0f);
	//ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-5, 0, 7));
	//glUniformMatrix4fv(glGetUniformLocation(vrShader.getShaderProgram(), "matrix"), 1, GL_FALSE, value_ptr(ViewMatrix * ModelMatrix));
	//firstModel.render(vrShader.getShaderProgram());

	glUseProgram(proteinShader.getShaderProgram());
	ModelMatrix = glm::mat4(1.0f);
	//make model matrix smaller

	ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-5, 0, 15));
	ModelMatrix = glm::rotate(ModelMatrix, (float)glfwGetTime() * 0.2f, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
	glUniformMatrix4fv(glGetUniformLocation(proteinShader.getShaderProgram(), "matrix"), 1, GL_FALSE, value_ptr(ViewMatrix * ModelMatrix));
	firstProtein.render(proteinShader.getShaderProgram());
	
	
	glUseProgram(controllerShader.getShaderProgram());
	vrLoader.renderControllers(controllerShader.getShaderProgram(), ViewMatrix);

	glUseProgram(quad3DShader.getShaderProgram());
	std::vector<glm::mat4> vrWindowMatrices;

	//quadTest.loadTexture(testBuffer.m_nRenderTextureId, 200, 200);
	ModelMatrix = glm::mat4(1.0f);
	ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0, 0, 5));
	ModelMatrix = glm::rotate(ModelMatrix, (float)glfwGetTime() * 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));
	vrWindowMatrices.push_back(ViewMatrix * ModelMatrix);

	ModelMatrix = glm::mat4(1.0f);
	ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0, 0, 5));
	ModelMatrix = glm::rotate(ModelMatrix, (float)glfwGetTime() * 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));
	vrWindowMatrices.push_back(ViewMatrix * ModelMatrix);
	guiLoader.renderVRGui(&vrWindowMatrices);


	//glUniformMatrix4fv(glGetUniformLocation(quad3DShader.getShaderProgram(), "matrix"), 1, GL_FALSE, value_ptr(ViewMatrix * ModelMatrix));
	//quadTest.render(quad3DShader.getShaderProgram());

	//guiLoader.renderVRGui(ViewMatrix);
}



void renderCompanionWindow() {
//-----------------------------------------------------------------------------
// Purpose: Renders the Companion window (on the monitor) with the left eye's
// 		rendered texture.
//
// Returns: N/A
//-----------------------------------------------------------------------------
	glViewport(0, 0, screenWidth, screenHeight);
	quad.loadTexture(LeftEyeFrameBuffer.m_nRenderTextureId, vrLoader.renderWidth, vrLoader.renderHeight);
	glUseProgram(quadShader.getShaderProgram());
	quad.render();
}

void display() {
//-----------------------------------------------------------------------------
// Purpose: The main rendering loop. This function is called every frame.
//
// Returns: N/A
//-----------------------------------------------------------------------------
	vrLoader.refresh(); //updates positions of the openVR devices.

	//Projection Matrix for screen.
	//glUniformMatrix4fv(glGetUniformLocation(vrShader.getShaderProgram(), "ProjectionMatrix"), 1, GL_FALSE, &ProjectionMatrix[0][0]);

	//glDisable(GL_DEPTH_TEST);
	//glBindFramebuffer(GL_FRAMEBUFFER, testBuffer.m_nRenderFramebufferId);
	//glViewport(0, 0, 200, 200);
	//guiLoader.renderWindowGui();
	////renderAll(ProjectionMatrix * camera.getMatrix());
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glEnable(GL_MULTISAMPLE);//enables anti ailiasing
	glEnable(GL_DEPTH_TEST);

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
	

	//glViewport(0, 0, screenWidth, screenHeight);
	//so we can use the same shader for the VR headset and the companion window, we multiply the projection matrix by the view matrix here rather than in the shader.
	//renderAll(ProjectionMatrix * camera.getMatrix());

	//Code to make the companion window display view from headset location - George
	//renderAll(ProjectionMatrix * vrLoader.getHeadsetMatrix());
	
	//Render to the companion window. (Used for if we were putting the headset's eyes quad onto the screen as a texture. Not implemented yet) 
	if (guiLoader.getCameraMode() == MONITOR_VIEW) {
		glViewport(0, 0, screenWidth, screenHeight);
		renderAll(ProjectionMatrix * camera.getMatrix());
		glDisable(GL_DEPTH_TEST);
	}
	else {
		glDisable(GL_DEPTH_TEST);
		renderCompanionWindow();
	}

	// Turn framebuffer texture into an OpenVR texture.
	vr::Texture_t leftEyeTexture = { (void*)(uintptr_t)LeftEyeFrameBuffer.m_nRenderTextureId, vr::TextureType_OpenGL, vr::ColorSpace_Gamma };
	vr::Texture_t rightEyeTexture = { (void*)(uintptr_t)RightEyeFrameBuffer.m_nRenderTextureId, vr::TextureType_OpenGL, vr::ColorSpace_Gamma };

	//Render the texture to the headset.
	vrLoader.render(leftEyeTexture, rightEyeTexture);
	guiLoader.renderWindowGui();

	//Clear all buffers.
	glFlush();

}

void reshape(GLFWwindow* window, int width, int height) {
//-----------------------------------------------------------------------------
// Purpose: Explains to openGL what size window we are rendering to. 
//			Creates a projection matrix based off of the window size.
// 
// Returns: n/a
//-----------------------------------------------------------------------------
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

void GLAPIENTRY
messageCallback(GLenum source,
	GLenum type,
	unsigned int id,
	GLenum severity,
	GLsizei length,
	const char* message,
	const void* userParam)
{
//-----------------------------------------------------------------------------
// Purpose: Debugging function we found on web to help us resolve errors.
//			it wont be in the final product. Prints errors in human readable format.
//		
// Author: https://learnopengl.com/In-Practice/Debugging	
// 
// Returns: n/a
//-----------------------------------------------------------------------------
	
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	std::cout << "---------------" << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
	case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
	} std::cout << std::endl;

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
	case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
	} std::cout << std::endl;

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
	case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
	} std::cout << std::endl;
	std::cout << std::endl;
}

int initModels() {
//-----------------------------------------------------------------------------
// Purpose: Loads and compiles models/proteins.
// 
// Returns: (In future) 1 or 0 depending on whether the program ran successfully or not. 
//-----------------------------------------------------------------------------
	objLoader.loadOBJ("src/models/teapot.obj");
	firstModel.loadModelFromObj(objLoader);
	firstModel.compileModel();

	proteinLoader.loadProtein("src/proteins/1ADG7046.pdb");
	firstProtein.loadProteinFromProteinLoader(proteinLoader);
	firstProtein.compileModel();

	quad.compileQuad();
	quadTest.compileQuad();

	return 0;
}


int init() {
//-----------------------------------------------------------------------------
// Purpose: Initialise all the libraries, models, shaders.
// 
// Returns: 1 or 0 depending on whether the initialisation was successful.
//-----------------------------------------------------------------------------
	
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

	//Initialise GLEW and check for errors. (GLEW is a library allowing use of opengl with languages other than c)
	glewExperimental = GL_TRUE;
	err = glewInit();
	if (GLEW_OK != err)
	{
		std::cout << "Error: " << glewGetErrorString(err) << std::endl;
		glfwTerminate();
	}




	reshape(window, screenWidth, screenHeight);
	glfwSetFramebufferSizeCallback(window, reshape); //if window size changes, reshape the viewport et.c using "reshape()", 
	glfwSetKeyCallback(window, key_callback);
	glfwSwapInterval(0);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	

	vrShader.createShaderFromFile("src/shaders/basicvr.shader");
	controllerShader.createShaderFromFile("src/shaders/basicvr_texture.shader");
	proteinShader.createShaderFromFile("src/shaders/basicvr_colour.shader");
	quadShader.createShaderFromFile("src/shaders/quad.shader");
	quad3DShader.createShaderFromFile("src/shaders/quad3d.shader");

	if (vrLoader.initVR() != 0) {
		std::cout << "Failed to setup VR." << std::endl;
		return -1;
	}

	//create frame buffers. these are used to store the images/textures that display to the left and right eyes.
	LeftEyeFrameBuffer.createFrameBuffer(vrLoader.renderWidth, vrLoader.renderHeight);
	RightEyeFrameBuffer.createFrameBuffer(vrLoader.renderWidth, vrLoader.renderHeight);
	//testBuffer.createFrameBuffer(200, 200);
	//createFrameBuffer(vrLoader.renderWidth, vrLoader.renderHeight, LeftEyeFrameBuffer);
	//createFrameBuffer(vrLoader.renderWidth, vrLoader.renderHeight, RightEyeFrameBuffer);
	//createFrameBuffer(200, 200, testBuffer);


	if (initModels() != 0) {
		std::cout << "Failed to load models." << std::endl;
		return -1;
	}

	//set up the keyboard controlled camera and set its initial position.
	//camera = Camera();
	camera.setPosition(glm::vec3(0.0, 0.0, 0.0));
	camera.setDirection(glm::vec3(0.0, -3.0, 0.0));
	

	glEnable(GL_DEBUG_OUTPUT);//enables a debugging mode. if theres an error in any opengl code it will run the message callback function.
	glDebugMessageCallback(messageCallback, 0);

	if (guiLoader.initGLFWGui(window) != 0) {
		std::cout << "Failed to initialise GUI." << std::endl;
			return -1;
	}
	if (guiLoader.initVRGui(quad3DShader.getShaderProgram()) != 0) {
		std::cout << "Failed to initialise VR GUI." << std::endl;
		return -1;
	}

}

int main()
{
//-----------------------------------------------------------------------------
// Purpose: First function which runs in the program. 
// 
// Returns: 1 or 0 depending on whether the program ran successfully or not.
//-----------------------------------------------------------------------------

	//Initialisation function.
	init();



	// Loop until the user closes the window
	while (!glfwWindowShouldClose(window))
	{

		// Main render loop.
		display();

		//Handling Controller Input.
		vrLoader.handleInput();

		

		// Swap front and back buffers. Puts the complete frame on screen once its completed rendering.
		glfwSwapBuffers(window);

		// Poll for and process events (keyboard + mouse input stuff)
		glfwPollEvents();

	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}
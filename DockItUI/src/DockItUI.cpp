// DockItUI.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <openvr.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "OBJLoader.h"
#include "Shader.h"
#include "Model3D.h"

GLFWwindow* window;
GLenum err;
int screenWidth = 800;
int screenHeight = 600;
glm::mat4 ProjectionMatrix;
glm::mat4 ModelViewMatrix;
Shader myShader;
vr::IVRSystem* pHMD;
OBJLoader objLoader;
Model3D firstModel;

void renderControllers() {

}

void renderAll() {
	firstModel.render(myShader);
}

void renderCompanionWindow() {

}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(myShader.getShaderProgram());

	renderAll();
	renderControllers();
	renderCompanionWindow();
	glFlush();

}

void reshape(GLFWwindow* window, int width, int height) {
	screenWidth = width;
	screenHeight = height;
	glViewport(0, 0, screenWidth, screenHeight);
	ProjectionMatrix = glm::perspective(glm::radians(45.0f), (GLfloat)screenWidth / (GLfloat)screenHeight, 1.0f, 200.0f);
}

int initModels() {
	objLoader.loadOBJ("src/models/teapot.obj");
	firstModel.loadModelFromObj(objLoader);
	return 0;
}

int initVR() {

	if (vr::VR_IsHmdPresent()) {
		std::cout << "HMD is present." << std::endl;
		if (vr::VR_IsRuntimeInstalled()) {
			std::cout << "OpenVR Runtime is installed." << std::endl;
		}
		else {
			std::cout << "OpenVR Runtime is not installed." << std::endl;
			return -1;
		}
	}
	else {
		std::cout << "HMD is not present." << std::endl;
		return -1;
	}

	//Load SteamVR Runtime

	vr::EVRInitError evrError = vr::VRInitError_None;
	pHMD = vr::VR_Init(&evrError, vr::VRApplication_Scene);

	if (evrError != vr::VRInitError_None) {
		std::cout << "Could not load runtime." << std::endl;
		return -1;
	}
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
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);

	myShader.createShaderFromFile("src/shaders/basic.shader");

	if (initVR() != 0) {
		std::cout << "Failed to setup VR." << std::endl;
		return -1;
	}


	if (initModels() != 0) {
		std::cout << "Failed to load models." << std::endl;
		return -1;
	}
}


int main()
{

	

	//Initialisation function.
	init();


	// Loop until the user closes the window
	while (!glfwWindowShouldClose(window))
	{

		display();

		// Swap front and back buffers
		glfwSwapBuffers(window);

		// Poll for and process events
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
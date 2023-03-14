// DockItUI.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <openvr.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <OBJLoader.h>
#include <Shader.h>

GLFWwindow* window;
GLenum err;
int screenWidth = 800;
int screenHeight = 600;
glm::mat4 ProjectionMatrix;
glm::mat4 ModelViewMatrix;
Shader shader;

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void reshape(GLFWwindow* window, int width, int height) {
	screenWidth = width;
	screenHeight = height;
	glViewport(0, 0, screenWidth, screenHeight);
	ProjectionMatrix = glm::perspective(glm::radians(45.0f), (GLfloat)screenWidth / (GLfloat)screenHeight, 1.0f, 200.0f);
}

void init() {

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);


}


int main()
{
	if (!glfwInit())
	{
		return -1;
	}

	window = glfwCreateWindow(screenWidth, screenHeight, "DockItUI", NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	err = glewInit();
	if (GLEW_OK != err)
	{
		std::cout << "Error: " << glewGetErrorString(err) << std::endl;
		glfwTerminate();
	}

	reshape(window, screenWidth, screenHeight);
	glfwSetFramebufferSizeCallback(window, reshape);

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


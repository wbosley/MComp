// DockItUI.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <openvr.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

//sample opengl window using glfw
int main()
{
	if (!glfwInit())
	{
		return -1;
	}

	GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// Loop until the user closes the window
	while (!glfwWindowShouldClose(window))
	{
		// Render here
		glClear(GL_COLOR_BUFFER_BIT);

		// Swap front and back buffers
		glfwSwapBuffers(window);

		// Poll for and process events
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

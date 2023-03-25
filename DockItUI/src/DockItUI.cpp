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
Model3D firstModel;
Camera camera;
struct FrameBuffer {
	GLuint m_nDepthBufferId;
	GLuint m_nRenderTextureId;
	GLuint m_nRenderFramebufferId;
};
FrameBuffer LeftEyeFrameBuffer;
FrameBuffer RightEyeFrameBuffer;
vr::TrackedDevicePose_t trackedDevicePose[vr::k_unMaxTrackedDeviceCount];
glm::mat4 mat4DevicePose[vr::k_unMaxTrackedDeviceCount];
glm::mat4 mat4HMDPose;
glm::mat4 ProjectionMatrixLeftEye;
glm::mat4 ProjectionMatrixRightEye;
glm::mat4 ViewMatrixLeftEye;
glm::mat4 ViewMatrixRightEye;


glm::mat4 getEyeProjectionMatrix(vr::Hmd_Eye eye) {
//-----------------------------------------------------------------------------
// Purpose: Gets the projection matrix for the specified eye.
// 
// Returns: ProjectionMatrix with respect to the specified eye.
//-----------------------------------------------------------------------------
	vr::HmdMatrix44_t matrix = pHMD->GetProjectionMatrix(eye, nearClip, farClip);
	return glm::mat4(
		matrix.m[0][0], matrix.m[1][0], matrix.m[2][0], matrix.m[3][0],
		matrix.m[0][1], matrix.m[1][1], matrix.m[2][1], matrix.m[3][1],
		matrix.m[0][2], matrix.m[1][2], matrix.m[2][2], matrix.m[3][2],
		matrix.m[0][3], matrix.m[1][3], matrix.m[2][3], matrix.m[3][3]
	);
}

glm::mat4 getEyeViewMatrix(vr::Hmd_Eye eye) {
//-----------------------------------------------------------------------------
// Purpose: Gets the view matrix for the specified eye.
//
// Returns: ViewMatrix with respect to the specified eye.
//-----------------------------------------------------------------------------
	vr::HmdMatrix34_t matrix = pHMD->GetEyeToHeadTransform(eye);
	glm::mat4 mat4EyePose = glm::mat4(
		matrix.m[0][0], matrix.m[1][0], matrix.m[2][0], 0.0,
		matrix.m[0][1], matrix.m[1][1], matrix.m[2][1], 0.0,
		matrix.m[0][2], matrix.m[1][2], matrix.m[2][2], 0.0,
		matrix.m[0][3], matrix.m[1][3], matrix.m[2][3], 1.0
	);
	return inverse(mat4EyePose);
}

glm::mat4 getEyeViewProjectionMatrix(vr::Hmd_Eye eye) {
//-----------------------------------------------------------------------------
// Purpose: Gets the combined view and projection matrix for the specified eye.
//
// Returns: View and Projection Matrix with respect to the specified eye.
//-----------------------------------------------------------------------------



	glm::mat4 matrix;
	if (eye == vr::Eye_Left) {
		matrix = ProjectionMatrixLeftEye * ViewMatrixLeftEye * mat4HMDPose;

	}
	else if (eye == vr::Eye_Right) {
		matrix = ProjectionMatrixRightEye * ViewMatrixRightEye * mat4HMDPose;
	}

	return matrix;

}

glm::mat4 convertSteamVRMatrix(const vr::HmdMatrix34_t& pose) {
//-----------------------------------------------------------------------------
// Purpose: Converts a SteamVR matrix to a glm matrix.
//			The SteamVR matrix is row-major, while the glm matrix is column-major.
// 
// Returns: glm matrix.
//-----------------------------------------------------------------------------
	glm::mat4 matrix(
		pose.m[0][0], pose.m[1][0], pose.m[2][0], 0.0,
		pose.m[0][1], pose.m[1][1], pose.m[2][1], 0.0,
		pose.m[0][2], pose.m[1][2], pose.m[2][2], 0.0,
		pose.m[0][3], pose.m[1][3], pose.m[2][3], 1.0f
	);
	return matrix;
}

void updateHMDMatrixPose() {
//-----------------------------------------------------------------------------
// Purpose: Updates the pose of the HMD.
// 
// Returns: N/A
//-----------------------------------------------------------------------------

// Calling WaitGetPoses gets the set of poses that are currently available, with an optional timeout of 0ms. In this case, that means we get the
// latest poses. If no poses are available, we'll just get the ones we got the last time through the loop. Waiting for poses is the key to
// getting good latency.
// The purpose of poses is to describe the offset of each device since the last frame. This lets you know how far each device has moved.


	vr::VRCompositor()->WaitGetPoses(trackedDevicePose, vr::k_unMaxTrackedDeviceCount, NULL, 0);

	mat4DevicePose[vr::k_unTrackedDeviceIndex_Hmd] = convertSteamVRMatrix(trackedDevicePose[vr::k_unTrackedDeviceIndex_Hmd].mDeviceToAbsoluteTracking);
	if (trackedDevicePose[vr::k_unTrackedDeviceIndex_Hmd].bPoseIsValid) {
		mat4HMDPose = mat4DevicePose[vr::k_unTrackedDeviceIndex_Hmd];
		mat4HMDPose = inverse(mat4HMDPose);
	}
}

void renderControllers() {

}

void renderAll(vr::Hmd_Eye eye) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.3f, 0.2f, 0.5f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	//ViewMatrix = camera.getMatrix();
	ModelMatrix = glm::mat4(1.0f);

	ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0, 0, 7));
	//ModelViewMatrix = ViewMatrix * ModelMatrix;
	//glUniformMatrix4fv(glGetUniformLocation(myShader.getShaderProgram(), "ModelViewMatrix") , 1, GL_FALSE, &ModelViewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(myShader.getShaderProgram(), "matrix"), 1, GL_FALSE, value_ptr(getEyeViewProjectionMatrix(eye) * ModelMatrix));
	firstModel.render(myShader);
	renderControllers();
}

void renderCompanionWindow() {

}

void display() {
	
	glUseProgram(myShader.getShaderProgram());
	updateHMDMatrixPose();

	//making projection matrix
	//glUniformMatrix4fv(glGetUniformLocation(myShader.getShaderProgram(), "ProjectionMatrix"), 1, GL_FALSE, &ProjectionMatrix[0][0]);
	//rendering our models.


	glEnable(GL_MULTISAMPLE);
	// Left eye
	
	glBindFramebuffer(GL_FRAMEBUFFER, LeftEyeFrameBuffer.m_nRenderFramebufferId);
	glViewport(0, 0, renderWidth, renderHeight);
	renderAll(vr::Eye_Left);
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );


	// Right eye
	glBindFramebuffer(GL_FRAMEBUFFER, RightEyeFrameBuffer.m_nRenderFramebufferId);
	glViewport(0, 0, renderWidth, renderHeight);
	renderAll(vr::Eye_Right);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glDisable(GL_MULTISAMPLE);
	
	renderCompanionWindow();


	// Submit the rendered texture to the headset.
	vr::Texture_t leftEyeTexture = { (void*)(uintptr_t)LeftEyeFrameBuffer.m_nRenderTextureId, vr::TextureType_OpenGL, vr::ColorSpace_Gamma };
	vr::VRCompositor()->Submit(vr::Eye_Left, &leftEyeTexture);
	vr::Texture_t rightEyeTexture = { (void*)(uintptr_t)RightEyeFrameBuffer.m_nRenderTextureId, vr::TextureType_OpenGL, vr::ColorSpace_Gamma };
	vr::VRCompositor()->Submit(vr::Eye_Right, &rightEyeTexture);



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
	
	vr::EVRInitError cError = vr::VRInitError_None;

	if (!vr::VRCompositor()) {
		std::cout << "Could not load Compositor." << std::endl;
		return -1;
	}
	
	vr::EVRInputError error = vr::VRInputError_None;

	error = vr::VRInput()->SetActionManifestPath("src/hellovr_bindings_generic.json");


	//I assume this is getting the render size of headset's eyes.
	pHMD->GetRecommendedRenderTargetSize(&renderWidth, &renderHeight);

	//Create frame buffer for left and right eye using the render size.
	createFrameBuffer(renderWidth, renderHeight, LeftEyeFrameBuffer);
	createFrameBuffer(renderWidth, renderHeight, RightEyeFrameBuffer);

	//Setup eye matrices.

	ProjectionMatrixLeftEye = getEyeProjectionMatrix(vr::Eye_Left);
	ProjectionMatrixRightEye = getEyeProjectionMatrix(vr::Eye_Right);

	ViewMatrixLeftEye = getEyeViewMatrix(vr::Eye_Left);
	ViewMatrixRightEye = getEyeViewMatrix(vr::Eye_Right);

	mat4HMDPose = glm::mat4(1.0f);

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

	myShader.createShaderFromFile("src/shaders/basic.shader");

	if (initVR() != 0) {
		std::cout << "Failed to setup VR." << std::endl;
		return -1;
	}


	if (initModels() != 0) {
		std::cout << "Failed to load models." << std::endl;
		return -1;
	}

	camera = Camera();


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
#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "openvr.h"
#include <vector>

class Quad3D
{

	private:
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> uvs;
		GLuint texture;
		uint16_t texWidth;
		uint16_t texHeight;
		GLuint VAO;
		GLuint* VBOs;
	public:
		Quad3D();
		~Quad3D();
		int loadTexture(GLuint textureId, uint16_t texWidth, uint16_t texHeight);
		int compileQuad();
		int render();
};


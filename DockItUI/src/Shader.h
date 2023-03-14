#pragma once
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <iostream>
#include <sstream>
#include <fstream>

class Shader
{


public:
	Shader();
	~Shader();
	int createShaderFromFile(const char* filePath);
	GLuint getShaderProgram();

private:
	std::string vertexShaderSrc;
	std::string fragmentShaderSrc;
};


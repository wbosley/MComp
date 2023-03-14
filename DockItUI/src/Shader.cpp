#include "Shader.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <iostream>
#include <sstream>
#include <fstream>

GLuint program;

Shader::Shader()
{
}

Shader::~Shader()
{
}

int Shader::createShaderFromFile(const char* filePath) {
	std::ifstream stream(filePath);

	enum class ShaderType {
		NONE = -1,
		VERTEX = 0,
		FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];//string stream is an array. 2 string streams: 1 for vertex shader, 1 for fragment shader
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line)) {
		//basically if not null
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos) {
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos) {
				type = ShaderType::FRAGMENT;
			}
		}
		else {
			//if its not a shader, bitwise shift the line into the correct slot in the string stream array.
			ss[(int)type] << line << '\n';
		}
	}

	vertexShaderSrc = ss[0].str();
	fragmentShaderSrc = ss[1].str();

	program = glCreateProgram();

	GLuint shaderId = glCreateShader(GL_VERTEX_SHADER);
	const char* src = vertexShaderSrc.c_str();
	glShaderSource(shaderId, 1, &src, nullptr);
	glCompileShader(shaderId);
	glAttachShader(program, shaderId);
	glDeleteShader(shaderId);


	shaderId = glCreateShader(GL_FRAGMENT_SHADER);
	src = fragmentShaderSrc.c_str();
	glShaderSource(shaderId, 1, &src, nullptr);
	glCompileShader(shaderId);
	glAttachShader(program, shaderId);
	glDeleteShader(shaderId);

	glLinkProgram(program);
	glValidateProgram(program);

	return 0;
}

GLuint Shader::getShaderProgram() {
	return program;
}


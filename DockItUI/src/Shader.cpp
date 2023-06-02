#include "Shader.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <iostream>
#include <sstream>
#include <fstream>



Shader::Shader()
{
}

Shader::~Shader()
{
}

GLuint Shader::compileShader(GLuint type, const char* shaderSrc) {
	GLuint shaderId = glCreateShader(type);
	glShaderSource(shaderId, 1, &shaderSrc, nullptr);
	glCompileShader(shaderId);

	//checking the shader compiled successfully
	int success;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE) {
		int length;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(shaderId, length, &length, message);
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;
	}

	return shaderId;
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

	//separate shaders into two strings
	this->vertexShaderSrc = ss[0].str();
	this->fragmentShaderSrc = ss[1].str();

	this->program = glCreateProgram();

	GLuint vs = compileShader(GL_VERTEX_SHADER, vertexShaderSrc.c_str());
	GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSrc.c_str());

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	std::cout << "Shader compiled successfully!" << std::endl;

	return 0;
}

GLuint Shader::getShaderProgram() {
	return this->program;
}


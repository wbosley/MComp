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

int Shader::createShaderFromFile(char* filePath) {
	std::ifstream stream(filePath);

	enum class ShaderType {
		NONE = -1,
		VERTEX = 0,
		FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line)) {
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos) {
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos) {
				type = ShaderType::FRAGMENT;
			}
		}
		else {
			ss[(int)type] << line << std::endl;
		}
	}
}
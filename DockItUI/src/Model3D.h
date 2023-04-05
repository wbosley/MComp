#pragma once
#include "OBJLoader.h"
#include "Shader.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
class Model3D
{

	//constructor and destructor
	public:
		Model3D();
		~Model3D();

		//will return 1 or zero depending on failure or success
		int loadModelFromObj(OBJLoader obj);
		int render(Shader myShader);
		

	protected:
		//these are in instance variables for a threedeeobjects
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> normals;

		std::vector<unsigned int> vertices_indices;
		std::vector<unsigned int> uvs_indices;
		std::vector<unsigned int> normals_indices;

		void updateBufferData();

		int putModelDataInVbosAndVaos();

};


#pragma once
#include "OBJLoader.h"
class Model3D
{

	//constructor and destructor
	public:
		Model3D();
		~Model3D();

		//will return 1 or zero depending on failure or success
		int loadModelFromObj(OBJLoader obj);

	private:
		//these are in instance variables for a threedeeobjects
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> normals;

		std::vector<unsigned int> vertices_indices;
		std::vector<unsigned int> uvs_indices;
		std::vector<unsigned int> normals_indices;

};


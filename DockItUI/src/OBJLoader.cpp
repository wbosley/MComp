#include "OBJLoader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <Windows.h>

OBJLoader::OBJLoader() {

}

//currenrtly we are just storing stuff in the obj loader object. its not returning anything.
//todo - could make it so you pass a objloader to a function in the threedeemodel class which returns the info. create a class for a 3d model which does nothing, which creates a 3d model from objloader
//creat an objinfo class which has an object with the variables. the struct is created in objloader, and its returned.
bool OBJLoader::loadOBJ(const char* path) {

	vertices.clear();
	uvs.clear();
	normals.clear();

	vertices_indices.clear();
	uvs_indices.clear();
	normals_indices.clear();

	std::ifstream file;
	std::string line;
	std::string offload;
	file.open(path, std::ios::in);

	if (file.is_open()) {
		std::cout << "File opened successfully." << std::endl;
	}
	else {
		std::cout << "File failed to open." << std::endl;
		return false;
	}
	while (getline(file, line)) {
		//make a substring of the line and deliminating using spaces. we are getting the first letter, the v, to work out if its a vertex etc.
		std::string identifier = line.substr(0, line.find(" "));
		//stream  -stream of input. we are taking the line, bitwising shifting into offload. we are deliminating by default delimter (space) - its etting rid of the v, and gettin the first vertex number. v and space go into offload, line turns into just the numbers.
		std::stringstream ss(line);
		ss >> offload;
		std::string test1, test2, test3;

		std::vector<std::string> indicies;

		if (identifier == "v") {
			glm::vec3 vertex;
			//shift the next three parts into vertex.x, vertex.y and vertex.z
			ss >> vertex.x;
			ss >> vertex.y;
			ss >> vertex.z;
			//putting the vertex into the array of vertices
			this->vertices.push_back(vertex);
		}
		else if (identifier == "vt") {
			glm::vec2 uv;
			ss >> uv.x;
			ss >> uv.y;
			this->uvs.push_back(uv);
		}
		else if (identifier == "vn") {
			glm::vec3 normal;
			ss >> normal.x;
			ss >> normal.y;
			ss >> normal.z;
			this->normals.push_back(normal);
		}
		else if (identifier == "f") {
			int vI[3], uI[3], nI[3], i = 0, j = 0;

			for (int i = 0; i < 3; i++) {
				ss >> offload;
				indicies.push_back(offload);
			}
			for (auto val : indicies) {
				std::stringstream ss2(val);
				std::string dump;
				getline(ss2, dump, '/');
				this->vertices_indices.push_back(std::stoi(dump) - 1);
				getline(ss2, dump, '/');
				this->uvs_indices.push_back(std::stoi(dump));
				getline(ss2, dump, '/');
				this->normals_indices.push_back(std::stoi(dump));
			}
		}
	}
	file.close();

	/*for (int i = 0; i < vertices_indices.size(); i++) {
		int vertexIndex = vertices_indices[i];
		glm::vec3 vertex = vertices[vertexIndex - 1];
		vertices2.push_back(vertex);
	}*/
	return true;
}
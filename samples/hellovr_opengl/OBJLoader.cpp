#include "OBJLoader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <Windows.h>

OBJLoader::OBJLoader() {

}

bool OBJLoader::loadOBJ(const char* path) {

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
		std::string identifier = line.substr(0, line.find(" "));
		
		std::stringstream ss(line);
		ss >> offload;
		std::string test1, test2, test3;

		std::vector<std::string> indicies;

		if (identifier == "v") {
			glm::vec3 vertex;
			ss >> vertex.x;
			ss >> vertex.y;
			ss >> vertex.z;
			//std::cout << "x:" << vertex.x << " y: " << vertex.y << " z: " << vertex.z << std::endl;
			vertices.push_back(vertex);
		}
		else if (identifier == "vt") {
			glm::vec2 uv;
			ss >> uv.x;
			ss >> uv.y;
			uvs.push_back(uv);
		}
		else if (identifier == "vn") {
			glm::vec3 normal;
			ss >> normal.x;
			ss >> normal.y;
			ss >> normal.z;
			normals.push_back(normal);
		}
		else if (identifier == "f") {
			int vI[3], uI[3], nI[3], i = 0, j = 0;
			std::string faces;

			for (int i = 0; i < 3; i++) {
				ss >> offload;
				indicies.push_back(offload);
			}
			for (auto val : indicies) {
				std::cout << val << std::endl;
				std::stringstream ss2(val);
				std::string dump;
				getline(ss2, dump, '/');
				vertices_indices.push_back(std::stoi(dump));
				getline(ss2, dump, '/');
				uvs_indices.push_back(std::stoi(dump));
				getline(ss2, dump, '/');
				normals_indices.push_back(std::stoi(dump));
			}
		}
	}
	file.close();
	return true;
}
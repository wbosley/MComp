#pragma once
#include <vector>
#include "../glm/glm/glm.hpp"
#include <fstream>
#include <iostream>
#include <sstream>


class ProteinLoader {
public:
	ProteinLoader();
	bool loadProtein(const char* path);

    //these are in instance variables for a protein
    std::vector<std::string> atomNames;
    std::vector<glm::vec3> coordinates;
    

  
};
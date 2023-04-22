#pragma once
#include <vector>
#include "../glm/glm/glm.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include "DataTable.h"


class ProteinLoader {
public:
	ProteinLoader();
	bool loadProtein(const char* path);

    //these are in instance variables for a protein
    std::vector<std::string> atomNames;
    std::vector<glm::vec3> atomCoordinates;
    std::vector<glm::vec3> atomColours;
    std::vector<float> atomRadius;

private:
    DataTables* data;
    DataTables::colour_list listOfColours;
    DataTables::VDW_Radii_list listOfRadii;
    std::string getAtomNameFromLine(std::string line);
    glm::vec3 getAtomCoordinatesFromLine(std::string line);
    glm::vec3 getColorFromAtomName(std::string atomName);
    float getRadiusFromAtomName(std::string atomName);
};
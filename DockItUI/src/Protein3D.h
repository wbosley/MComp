#pragma once
#include "Model3D.h"
#include "ProteinLoader.h"
#include "Sphere.h"
class Protein3D
{
	public :
		Protein3D();
		~Protein3D();
		int loadProteinFromProteinLoader(ProteinLoader proteinLoader);
		int render(Shader shader);
		int compileModel();

	private:
		std::vector<std::string> atomNames;
		std::vector<glm::vec3> atomCoordinates;
		std::vector<glm::vec3> atomColours;
		std::vector<float> atomRadius;

		std::vector<Sphere> spheres;
		Model3D protein;
		int constructSpheres();

};


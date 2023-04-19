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
		int render(Shader myShader);

	private:
		std::vector<std::string> atomNames;
		std::vector<glm::vec3> atomCoordinates;
		std::vector<glm::vec3> atomColours;
		std::vector<float> atomRadii;

		std::vector<Sphere> spheres;
		int constructSpheres();
	//constructspheres

	//render- loop through spheres and use their render function from model3d

};


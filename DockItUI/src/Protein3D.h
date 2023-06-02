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
		int render(GLuint shader);
		int compileModel();
		std::string getAtomName(int index);
		Model3D protein;

		std::vector<glm::vec3> boundBoxVerts;

	private:
		std::vector<std::string> atomNames;
		std::vector<glm::vec3> atomCoordinates;
		std::vector<glm::vec3> atomColours;
		std::vector<float> atomRadius;
		std::vector<Sphere> spheres;
		std::vector<glm::vec3> boundBoxFaces;
		int constructSpheres();
		int sphereVertSize = 0;

};


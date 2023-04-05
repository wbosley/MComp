#include "Protein3D.h"

//constructor
Protein3D::Protein3D()
{
}

//destructor
Protein3D::~Protein3D()
{
}

int Protein3D::loadProteinFromProteinLoader(ProteinLoader proteinLoader) {
	atomNames = proteinLoader.atomNames;
	atomCoordinates = proteinLoader.atomCoordinates;
	atomColours = proteinLoader.atomColours;
	atomRadii = proteinLoader.atomRadii;
	constructSpheres();

	return 0;
}

int Protein3D::constructSpheres() {
	for (int i = 0; i < atomNames.size(); i++) {
		//std::cout << "constructing sphere number " << i << std::endl;
		//std::cout << "radius: " << atomRadii[i] << std::endl;
		//std::cout << "centre: " << atomCoordinates[i].x << " " << atomCoordinates[i].y << " " << atomCoordinates[i].z << std::endl;
		Sphere sphere;
		sphere.setRadius(atomRadii[i] / 10.0f);
		sphere.constructGeometry(20);
		sphere.setCentre(atomCoordinates[i]);
		spheres.push_back(sphere);
	}

	return 0;
}

int Protein3D::render(Shader myShader) {
	//for (int i = 0; i < spheres.size(); i++) {
	//	spheres[i].render(myShader);
	//	//std::cout << "rendering spher number " << i << std::endl;
	//}

	for (Sphere sphere : spheres) {
		sphere.render(myShader);
	}

	return 0;
}


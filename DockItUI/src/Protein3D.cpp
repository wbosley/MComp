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
	atomRadius = proteinLoader.atomRadius;
	constructSpheres();

	return 0;
}

int Protein3D::constructSpheres() {
	for (int i = 0; i < atomNames.size(); i++) {
		Sphere sphere;
		sphere.setRadius(atomRadius[i] / 11.0f);
		sphere.constructGeometry(15);
		sphere.setCentre(atomCoordinates[i]);
		sphere.setColour(atomColours[i]);
		protein.addModelToMesh(sphere);
	}

	return 0;
}

int Protein3D::compileModel() {
	protein.compileModel();
	return 0;
}

int Protein3D::render(GLuint shader) {

	protein.render(shader);

	return 0;
}


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
		Sphere sphere;
		sphere.setRadius(atomRadii[i] / 5.0f);
		sphere.constructGeometry(15);
		sphere.setCentre(atomCoordinates[i]);
		protein.addModelToMesh(sphere);
	}
	protein.compileMesh();

	return 0;
}

int Protein3D::render(Shader shader) {

	protein.render(shader);

	return 0;
}


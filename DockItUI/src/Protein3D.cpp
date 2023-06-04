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
	atomNames = proteinLoader.atomNames;//populating the Protein3D's fields
	atomCoordinates = proteinLoader.atomCoordinates;
	atomColours = proteinLoader.atomColours;
	atomRadius = proteinLoader.atomRadius;
	constructSpheres();//make spheres representing each atom

	return 0;
}

int Protein3D::constructSpheres() {
	//for each aotm, make a sphere using its information, and then combine into
	//one big Model3d. This is the protein.
	for (int i = 0; i < atomNames.size(); i++) {
		Sphere sphere;
		sphere.setRadius(atomRadius[i] / 11.0f);
		sphereVertSize = sphere.constructGeometry(12);
		sphere.setCentre(atomCoordinates[i]);
		sphere.setColour(atomColours[i]);
		protein.addModelToMesh(sphere);
	}

	return 0;
}

int Protein3D::compileModel() {
	//compile the Model3D (named protein), and make the bounding box.
	protein.compileModel();
	std::vector<glm::vec3> verts = *protein.getVertices();
	glm::vec3 min_point = verts[0];
	glm::vec3 max_point = verts[0];
	for (glm::vec3 point : verts) {
		for (int i = 0; i < 3; i++) {
			min_point[i] = std::min(min_point[i], point[i]);
			max_point[i] = std::max(max_point[i], point[i]);
		}
	}

	boundBoxFaces.push_back(glm::vec3(min_point.x, min_point.y, min_point.z));
	boundBoxFaces.push_back(glm::vec3(max_point.x, min_point.y, min_point.z));
	boundBoxFaces.push_back(glm::vec3(max_point.x, max_point.y, min_point.z));
	boundBoxFaces.push_back(glm::vec3(min_point.x, max_point.y, min_point.z));
	boundBoxFaces.push_back(glm::vec3(min_point.x, min_point.y, max_point.z));
	boundBoxFaces.push_back(glm::vec3(max_point.x, min_point.y, max_point.z));
	boundBoxFaces.push_back(glm::vec3(max_point.x, max_point.y, max_point.z));
	boundBoxFaces.push_back(glm::vec3(min_point.x, max_point.y, max_point.z));


	boundBoxVerts.push_back(boundBoxFaces[0]);
	boundBoxVerts.push_back(boundBoxFaces[1]);
	boundBoxVerts.push_back(boundBoxFaces[2]);

	boundBoxVerts.push_back(boundBoxFaces[2]);
	boundBoxVerts.push_back(boundBoxFaces[3]);
	boundBoxVerts.push_back(boundBoxFaces[0]);

	// Right face
	boundBoxVerts.push_back(boundBoxFaces[1]);
	boundBoxVerts.push_back(boundBoxFaces[5]);
	boundBoxVerts.push_back(boundBoxFaces[6]);

	boundBoxVerts.push_back(boundBoxFaces[6]);
	boundBoxVerts.push_back(boundBoxFaces[2]);
	boundBoxVerts.push_back(boundBoxFaces[1]);

	// Back face
	boundBoxVerts.push_back(boundBoxFaces[5]);
	boundBoxVerts.push_back(boundBoxFaces[4]);
	boundBoxVerts.push_back(boundBoxFaces[7]);

	boundBoxVerts.push_back(boundBoxFaces[7]);
	boundBoxVerts.push_back(boundBoxFaces[6]);
	boundBoxVerts.push_back(boundBoxFaces[5]);

	// Left face
	boundBoxVerts.push_back(boundBoxFaces[4]);
	boundBoxVerts.push_back(boundBoxFaces[0]);
	boundBoxVerts.push_back(boundBoxFaces[3]);

	boundBoxVerts.push_back(boundBoxFaces[3]);
	boundBoxVerts.push_back(boundBoxFaces[7]);
	boundBoxVerts.push_back(boundBoxFaces[4]);

	// Top face
	boundBoxVerts.push_back(boundBoxFaces[3]);
	boundBoxVerts.push_back(boundBoxFaces[2]);
	boundBoxVerts.push_back(boundBoxFaces[6]);

	boundBoxVerts.push_back(boundBoxFaces[6]);
	boundBoxVerts.push_back(boundBoxFaces[7]);
	boundBoxVerts.push_back(boundBoxFaces[3]);

	// Bottom face
	boundBoxVerts.push_back(boundBoxFaces[4]);
	boundBoxVerts.push_back(boundBoxFaces[5]);
	boundBoxVerts.push_back(boundBoxFaces[1]);

	boundBoxVerts.push_back(boundBoxFaces[1]);
	boundBoxVerts.push_back(boundBoxFaces[0]);
	boundBoxVerts.push_back(boundBoxFaces[4]);

	return 0;
}

int Protein3D::render(GLuint shader) {

	protein.render(shader);

	return 0;
}

std::string Protein3D::getAtomName(int index) {
	return atomNames.at(ceil(index/sphereVertSize));
}


#include "Model3D.h"

//constructor
Model3D::Model3D()
{
}

//destructor
Model3D::~Model3D()
{
}

//load model from obj
int Model3D::loadModelFromObj(OBJLoader obj)
{
	//get the vertices from the objloader
	vertices = obj.vertices;
	//get the uvs from the objloader
	uvs = obj.uvs;
	//get the normals from the objloader
	normals = obj.normals;
	//get the vertices indices from the objloader
	vertices_indices = obj.vertices_indices;
	//get the uvs indices from the objloader
	uvs_indices = obj.uvs_indices;
	//get the normals indices from the objloader
	normals_indices = obj.normals_indices;
}
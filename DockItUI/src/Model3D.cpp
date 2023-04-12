#include "Model3D.h"



//constructor
Model3D::Model3D()
{
	isModelValid = false;
}

//destructor
Model3D::~Model3D()
{
}

int Model3D::loadOpenVRModel(std::vector<glm::vec3> vertices, std::vector<unsigned int> vertices_indices) {
	this->vertices = vertices;
	this->vertices_indices = vertices_indices;

	int vaoAndVboinitialised = putModelDataInVbosAndVaos();

	//adding together all the returned values to make sure ntohing broke. if more than 0 then something went wrong.
	return 0 + vaoAndVboinitialised;
}

//load model from obj
int Model3D::loadModelFromObj(OBJLoader obj)
{
	//copy the data from the objloader object into the model3d object
	this->vertices = obj.vertices;
	this->uvs = obj.uvs;
	this->normals = obj.normals;
	this->vertices_indices = obj.vertices_indices;
	this->uvs_indices = obj.uvs_indices;
	this->normals_indices = obj.normals_indices;

	int vaoAndVboinitialised = putModelDataInVbosAndVaos();

	//adding together all the returned values to make sure ntohing broke. if more than 0 then something went wrong.
	return 0 + vaoAndVboinitialised;

}

int Model3D::putModelDataInVbosAndVaos()
{
	VBOs = new GLuint[2];


	//make VBOs for vertices and indices
	glGenBuffers(2, this->VBOs);

	//making the VAO and binding it
	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);

	//put vertices in vbo
	glBindBuffer(GL_ARRAY_BUFFER, this->VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * this->vertices.size(), &this->vertices[0], GL_STATIC_DRAW);

	//put this VBO in the VAO, saying that we want it in the 0th location in the shader.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); //tells opengl how to retrieve VAO from memory. Inputs: What location in the shader this bound VBO is for,
	glEnableVertexAttribArray(0);

	//put indices in vbo
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->VBOs[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * this->vertices_indices.size(), &this->vertices_indices[0], GL_STATIC_DRAW);

	//put this VBO in the VAO, saying that we want it in the 0th location in the shader.
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0); //tells opengl how to retrieve VAO from memory. Inputs: What location in the shader this bound VBO is for,
	glEnableVertexAttribArray(1);

	//unbind the vbo and vao
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	isModelValid = true;

	return 0;
}

int Model3D::render(Shader myShader) 
{
	//bind the VAO
	glBindVertexArray(this->VAO);
	
	//glUseProgram(myShader.getShaderProgram());

	glDrawElements(GL_TRIANGLES, this->vertices_indices.size(), GL_UNSIGNED_INT, 0);
	
	glBindVertexArray(0);

	return 0;
}

void Model3D::updateBufferData()
{
	glBindBuffer(GL_ARRAY_BUFFER, this->VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * this->vertices.size(), &this->vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->VBOs[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * this->vertices_indices.size(), &this->vertices_indices[0], GL_STATIC_DRAW);
}
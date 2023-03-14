#include "Model3D.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <iostream>


GLuint VAO;
GLuint* VBOs;


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
	//copy the data from the objloader object into the model3d object
	vertices = obj.vertices;
	uvs = obj.uvs;
	normals = obj.normals;
	vertices_indices = obj.vertices_indices;
	uvs_indices = obj.uvs_indices;
	normals_indices = obj.normals_indices;

	int vaoAndVboinitialised = putModelDataInVbosAndVaos(vertices, vertices_indices);

	//adding together all the returned values to make sure ntohing broke. if more than 0 then something went wrong.
	return 0 + vaoAndVboinitialised;

}

int Model3D::putModelDataInVbosAndVaos(std::vector<glm::vec3> vertices, std::vector<unsigned int> vertices_indices)
{
	//make VBOs for vertices and indices
	glGenBuffers(2, VBOs);

	//making the VAO and binding it
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//put vertices in vbo
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	//put this VBO in the VAO, saying that we want it in the 0th location in the shader.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); //tells opengl how to retrieve VAO from memory. Inputs: What location in the shader this bound VBO is for,
	glEnableVertexAttribArray(0);


	//put indices in vbo
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices_indices.size(), &vertices_indices[0], GL_STATIC_DRAW);

	//put this VBO in the VAO, saying that we want it in the 0th location in the shader.
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0); //tells opengl how to retrieve VAO from memory. Inputs: What location in the shader this bound VBO is for,
	glEnableVertexAttribArray(1);

	//unbind the vbo and vao
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return 0;
}

int Model3D::render(Shader myShader) 
{
	//bind the VAO
	glBindVertexArray(VAO);

	//DONT HAVE A SHADER YET, uncomment when we have a shader :-)
	//glUseProgram(myShader);

	glDrawElements(GL_TRIANGLES, vertices_indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	return 0;
}
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

int Model3D::loadOpenVRModel(vr::RenderModel_t* vr_model, vr::RenderModel_TextureMap_t* vr_texture) {

	for (int i = 0; i < vr_model->unVertexCount; i++) {
		this->vertices.push_back(glm::vec3(vr_model->rVertexData[i].vPosition.v[0], vr_model->rVertexData[i].vPosition.v[1], vr_model->rVertexData[i].vPosition.v[2]));
	}

	//Convert vr_model indices into a vector of unsigned int
	for (int i = 0; i < vr_model->unTriangleCount * 3; i++) {
		this->vertices_indices.push_back(vr_model->rIndexData[i]);
	}

	//Convert vr_model texture coordinates into a vector of glm texture coordinates
	for (int i = 0; i < vr_model->unVertexCount; i++) {
		this->uvs.push_back(glm::vec2(vr_model->rVertexData[i].rfTextureCoord[0], vr_model->rVertexData[i].rfTextureCoord[1]));
	}
	this->textureData = vr_texture->rubTextureMapData;
	this->width = vr_texture->unWidth;
	this->height = vr_texture->unHeight;

	int vaoAndVboinitialised = putModelDataInVbosAndVaos(TEXTURES);

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

	int vaoAndVboinitialised = putModelDataInVbosAndVaos(NO_TEXTURES);

	//adding together all the returned values to make sure ntohing broke. if more than 0 then something went wrong.
	return 0 + vaoAndVboinitialised;

}

int Model3D::putModelDataInVbosAndVaos(RENDER_TYPE renderMode)
{
	if (renderMode == NO_TEXTURES) {
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
		//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0); //tells opengl how to retrieve VAO from memory. Inputs: What location in the shader this bound VBO is for,
		//glEnableVertexAttribArray(1);

		//unbind the vbo and vao
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);


	}
	else if (renderMode == TEXTURES) {
		VBOs = new GLuint[3];

		//make VBOs for vertices and indices
		glGenBuffers(3, this->VBOs);

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

		//put this VBO in the VAO, saying that we want it in the 1st location in the shader.
		//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0); //tells opengl how to retrieve VAO from memory. Inputs: What location in the shader this bound VBO is for,
		//glEnableVertexAttribArray(1);

		//put texture coordinates in vbo
		glBindBuffer(GL_ARRAY_BUFFER, this->VBOs[2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * this->uvs.size(), &this->uvs[0], GL_STATIC_DRAW);

		//put this VBO in the VAO, saying thawt we want it in the 2nd location in the shader.
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(2);

		//unbind the vbo and vao
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		glGenTextures(1, &this->texture);
		glBindTexture(GL_TEXTURE_2D, this->texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		GLfloat fLargest;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->textureData);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);

	}
	else {
		//i dunno what to do here
	}


	isModelValid = true;
	return 0;
}

int Model3D::render(Shader shader) 
{
	glUseProgram(shader.getShaderProgram());

	//bind the VAO
	glBindVertexArray(this->VAO);

	if (textureData != NULL) {
		glBindTexture(GL_TEXTURE_2D, this->texture);
		glDrawElements(GL_TRIANGLES, this->vertices_indices.size(), GL_UNSIGNED_INT, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else {
		glDrawElements(GL_TRIANGLES, this->vertices_indices.size(), GL_UNSIGNED_INT, 0);
	}

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
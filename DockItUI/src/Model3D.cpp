#include "Model3D.h"



//constructor
Model3D::Model3D()
{
	isModelCompiled = false;
	this->ModelMatrix = glm::mat4(1.0f);
}

//destructor
Model3D::~Model3D()
{
}

int Model3D::createQuadColour(glm::vec3 colour) {
	this->vertices.push_back(glm::vec3(-1.0f, -1.0f, 0.0f));
	this->vertices.push_back(glm::vec3(-1.0f, 1.0f, 0.0f));
	this->vertices.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
	this->vertices.push_back(glm::vec3(-1.0f, -1.0f, 0.0f));
	this->vertices.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
	this->vertices.push_back(glm::vec3(1.0f, -1.0f, 0.0f));

	this->colours.push_back(colour);
	this->colours.push_back(colour);
	this->colours.push_back(colour);
	this->colours.push_back(colour);
	this->colours.push_back(colour);
	this->colours.push_back(colour);

	this->vertices_indices.push_back(0);
	this->vertices_indices.push_back(1);
	this->vertices_indices.push_back(2);
	this->vertices_indices.push_back(3);
	this->vertices_indices.push_back(4);
	this->vertices_indices.push_back(5);

	this->render_type = COLOUR;
	return 0;
}

int Model3D::createQuad(GLuint textureId, bool is3D) {
	int textureWidth, textureHeight;

	glBindTexture(GL_TEXTURE_2D, textureId);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &textureWidth);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &textureHeight);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (is3D) {
		if (textureWidth == 400) {
			float quadWidth = textureWidth / 400.0f;
			float quadHeight = textureHeight / 400.0f;
			this->vertices.push_back(glm::vec3(-quadWidth, -quadHeight, 0.0f));
			this->vertices.push_back(glm::vec3(-quadWidth, quadHeight, 0.0f));
			this->vertices.push_back(glm::vec3(quadWidth, quadHeight, 0.0f));
			this->vertices.push_back(glm::vec3(-quadWidth, -quadHeight, 0.0f));
			this->vertices.push_back(glm::vec3(quadWidth, quadHeight, 0.0f));
			this->vertices.push_back(glm::vec3(quadWidth, -quadHeight, 0.0f));
		}
		else {
			this->vertices.push_back(glm::vec3(1.0f, 0.3f, 0.0f));
			this->vertices.push_back(glm::vec3(1.0f, -0.3f, 0.0f));
			this->vertices.push_back(glm::vec3(-1.0f, 0.3f, 0.0f));
			this->vertices.push_back(glm::vec3(1.0f, -0.3f, 0.0f));
			this->vertices.push_back(glm::vec3(-1.0f, 0.3f, 0.0f));
			this->vertices.push_back(glm::vec3(-1.0f, -0.3f, 0.0f));
		}


	}
	else {
		this->vertices.push_back(glm::vec3(-1.0f, -1.0f, 0.0f));
		this->vertices.push_back(glm::vec3(-1.0f, 1.0f, 0.0f));
		this->vertices.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
		this->vertices.push_back(glm::vec3(-1.0f, -1.0f, 0.0f));
		this->vertices.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
		this->vertices.push_back(glm::vec3(1.0f, -1.0f, 0.0f));
	}

	//create the indices
	this->vertices_indices.push_back(0);
	this->vertices_indices.push_back(1);
	this->vertices_indices.push_back(2);
	this->vertices_indices.push_back(3);
	this->vertices_indices.push_back(4);
	this->vertices_indices.push_back(5);


	this->uvs.push_back(glm::vec2(0.0f, 0.0f));
	this->uvs.push_back(glm::vec2(0.0f, 1.0f));
	this->uvs.push_back(glm::vec2(1.0f, 1.0f));
	this->uvs.push_back(glm::vec2(0.0f, 0.0f));
	this->uvs.push_back(glm::vec2(1.0f, 1.0f));
	this->uvs.push_back(glm::vec2(1.0f, 0.0f));

	this->texture = textureId;

	this->render_type = TEXTURES_PRECOMPILED;
	return 0;
}

int Model3D::createLine(glm::vec3 start, glm::vec3 end) {
	//create a line from start to end
	this->vertices.clear();
	this->vertices_indices.clear();
	this->colours.clear();
	this->vertices.push_back(start);
	this->vertices.push_back(end);
	this->vertices_indices.push_back(0);
	this->vertices_indices.push_back(1);
	this->colours.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
	this->colours.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
	this->render_type = COLOUR;
	return 0;
}

int Model3D::loadOpenVRModel(vr::RenderModel_t* vr_model, vr::RenderModel_TextureMap_t* vr_texture) {

	//Convert vr_model vertices into a vector of glm vertices
	for (int i = 0; i < vr_model->unVertexCount; i++) {
		this->vertices.push_back(glm::vec3(vr_model->rVertexData[i].vPosition.v[0], vr_model->rVertexData[i].vPosition.v[1], vr_model->rVertexData[i].vPosition.v[2]));
	}

	//Convert vr_model indices into a vector of unsigned int
	for (int i = 0; i < vr_model->unTriangleCount * 3; i++) {
		this->vertices_indices.push_back(vr_model->rIndexData[i]);
	}

	//Convert vr_model normals into a vector of glm::vec3
	for (int i = 0; i < vr_model->unVertexCount; i++) {
		this->normals.push_back(glm::vec3(vr_model->rVertexData[i].vNormal.v[0], vr_model->rVertexData[i].vNormal.v[1], vr_model->rVertexData[i].vNormal.v[2]));
	}

	//Convert vr_model texture coordinates into a vector of glm texture coordinates
	for (int i = 0; i < vr_model->unVertexCount; i++) {
		this->uvs.push_back(glm::vec2(vr_model->rVertexData[i].rfTextureCoord[0], vr_model->rVertexData[i].rfTextureCoord[1]));
	}
	this->textureData = vr_texture->rubTextureMapData;
	this->width = vr_texture->unWidth;
	this->height = vr_texture->unHeight;

	return 0;
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

	return 0;
}

int Model3D::changeColour(glm::vec3 colour) {

	//change the colour of the model
	this->colours.clear();
	for (int i = 0; i < this->vertices.size(); i++) {
		this->colours.push_back(colour);
	}
	this->render_type = COLOUR;
	return 0;
}

int Model3D::compileModel()
{
	//puts everything into the VAOS and VBOs for rendering.

	//number used for indexing the VBOs. IF normals have been provided with the model, then this number is 1, otherwise it is 0. This means that we can specifify whether to 3 or 4 VBOs by setting the number of vbos as 3 + isThereNormals. We can also use this to correctly acces indexs of specific vbos. for example, the index of the VBO containing textures is 2, but if there is normals, then it is 3. This means the index of the texture VBO is 2 + isThereNormals, which will be 2 if there are no normals, and 3 if there are normals.
	int isThereNormals = 1;
	if (this->normals.empty()) {
		isThereNormals = 0;
	}


	//check if texture data is present
	if (this->textureData != NULL) {
		render_type = TEXTURES;
		VBOs = new GLuint[3 + isThereNormals];
		glGenBuffers(4, this->VBOs);
	}
	else if (this->texture != NULL) {
		render_type = TEXTURES_PRECOMPILED;
		VBOs = new GLuint[3 + isThereNormals];
		glGenBuffers(4, this->VBOs);
	}
	else if (!this->colours.empty()) {
		render_type = COLOUR;
		VBOs = new GLuint[3 + isThereNormals];
		glGenBuffers(4, this->VBOs);
	}
	else {
		render_type = NO_TEXTURES;
		VBOs = new GLuint[2 + isThereNormals];
		glGenBuffers(3, this->VBOs);
	}

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

	if (isThereNormals){
		std::cout << "Normals size: " << this->normals.size() << std::endl;
		//put normals in vbo
		glBindBuffer(GL_ARRAY_BUFFER, this->VBOs[2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * this->normals.size(), &this->normals[0], GL_STATIC_DRAW);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0); //tells opengl how to retrieve VAO from memory. Inputs: What location in the shader this bound VBO is for,
		glEnableVertexAttribArray(1);
	}

	if (render_type == TEXTURES) {
		//put texture coordinates in vbo
		glBindBuffer(GL_ARRAY_BUFFER, this->VBOs[2 + isThereNormals]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * this->uvs.size(), &this->uvs[0], GL_STATIC_DRAW);

		//put this VBO in the VAO, saying thawt we want it in the 2nd location in the shader.
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(2);

		//Textures work differently. They arent bound to a vao, their bound to theyr own thing. We could bind a model and then bind an alternative texture if we wanted to. We dont assign a texture to a model when we make the VAOs.
		glGenTextures(1, &this->texture);
		glBindTexture(GL_TEXTURE_2D, this->texture);
		//set up how the textures will appear on the model.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		//todo check if we need this
		GLfloat fLargest;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);

		//this generates  the texture, using the textures width, height, and texture data.
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->textureData);
		glGenerateMipmap(GL_TEXTURE_2D); // this generates the mipmaps for the texture. Mipmaps are different versions of the texture at different sizes. This is so that the texture can be rendered at different sizes without looking bad.

		glBindTexture(GL_TEXTURE_2D, 0);

	}
	else if (this->render_type == TEXTURES_PRECOMPILED) {
		//put texture coordinates in vbo
		glBindBuffer(GL_ARRAY_BUFFER, this->VBOs[2 + isThereNormals]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * this->uvs.size(), &this->uvs[0], GL_STATIC_DRAW);

		//put this VBO in the VAO, saying thawt we want it in the 2nd location in the shader.
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(2);
	}
	else if (render_type == COLOUR) {
		glBindBuffer(GL_ARRAY_BUFFER, this->VBOs[2 + isThereNormals]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * this->colours.size(), &this->colours[0], GL_STATIC_DRAW);

		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0); //tells opengl how to retrieve VAO from memory. Inputs: What location in the shader this bound VBO is for,
		glEnableVertexAttribArray(2);//enables the number 2 location (where we are storing the colours) to be used
	}

	//unbind the vbo and vao
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	isModelCompiled = true;
	return 0;
}

int Model3D::addModelToMesh(Model3D model) {

	//add data from parameter model to this model
	for (int i = 0; i < model.vertices.size(); i++) {
		this->vertices.push_back(model.vertices[i]);
	}
	for (int i = 0; i < model.vertices_indices.size(); i++) {
		this->vertices_indices.push_back(model.vertices_indices[i] + this->vertices.size());
	}
	//add color data
	for (int i = 0; i < model.colours.size(); i++) {
		this->colours.push_back(model.colours[i]);
	}

	return 0;
}

int Model3D::render(GLuint shader) 
{
	glUseProgram(shader);



	//bind the VAO
	glBindVertexArray(this->VAO);

	if (this->render_type == TEXTURES || this->render_type == TEXTURES_PRECOMPILED) {
		glBindTexture(GL_TEXTURE_2D, this->texture);
		glDrawElements(GL_TRIANGLES, this->vertices_indices.size(), GL_UNSIGNED_INT, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else {
		if (this->vertices.size() <= 2) {
			glLineWidth(3.0f);
			glDrawElements(GL_LINES, this->vertices_indices.size(), GL_UNSIGNED_INT, 0);
		}
		else {
			glDrawElements(GL_TRIANGLES, this->vertices_indices.size(), GL_UNSIGNED_INT, 0);
		}
	}

	glBindVertexArray(0);

	return 0;
}

void Model3D::updateBufferData() //need to expand on this for batch rendering and controllers when their models change. (no models change at the moment
{
	glBindBuffer(GL_ARRAY_BUFFER, this->VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * this->vertices.size(), &this->vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->VBOs[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * this->vertices_indices.size(), &this->vertices_indices[0], GL_STATIC_DRAW);
}

std::vector<glm::vec3>* Model3D::getVertices() {
	return &this->vertices;
}
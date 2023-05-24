#include "Quad3D.h"

Quad3D::Quad3D()
{
	this->vertices.push_back(glm::vec3(-1.0f, -1.0f, 0.0f));
	this->vertices.push_back(glm::vec3(-1.0f, 1.0f, 0.0f));
	this->vertices.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
	this->vertices.push_back(glm::vec3(-1.0f, -1.0f, 0.0f));
	this->vertices.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
	this->vertices.push_back(glm::vec3(1.0f, -1.0f, 0.0f));

	//this->vertices.push_back(glm::vec3(-0.5f, -0.5f, 0.0f));
	//this->vertices.push_back(glm::vec3(-0.5f, 0.5f, 0.0f));
	//this->vertices.push_back(glm::vec3(0.5f, 0.5f, 0.0f));
	//this->vertices.push_back(glm::vec3(-0.5f, -0.5f, 0.0f));
	//this->vertices.push_back(glm::vec3(0.5f, 0.5f, 0.0f));
	//this->vertices.push_back(glm::vec3(0.5f, -0.5f, 0.0f));

	this->uvs.push_back(glm::vec2(0.0f, 0.0f));
	this->uvs.push_back(glm::vec2(0.0f, 1.0f));
	this->uvs.push_back(glm::vec2(1.0f, 1.0f));
	this->uvs.push_back(glm::vec2(0.0f, 0.0f));
	this->uvs.push_back(glm::vec2(1.0f, 1.0f));
	this->uvs.push_back(glm::vec2(1.0f, 0.0f));
}
Quad3D::~Quad3D()
{
}

int Quad3D::loadTexture(GLuint textureId, uint16_t texWidth, uint16_t texHeight) {
	this->texture = textureId;
	this->texWidth = texWidth;
	this->texHeight = texHeight;
	return 0;
}

int Quad3D::compileQuad() {
	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);
	VBOs = new GLuint[2];
	glGenBuffers(2, this->VBOs);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(glm::vec3), &this->vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, this->uvs.size() * sizeof(glm::vec2), &this->uvs[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	return 0;
}

int Quad3D::render() {
	//glUniformMatrix4fv(glGetUniformLocation(shader.getShaderProgram(), "matrix"), 1, GL_FALSE, value_ptr(matrix));
	glBindVertexArray(this->VAO);
	glBindTexture(GL_TEXTURE_2D, this->texture);
	glDrawArrays(GL_TRIANGLES, 0, this->vertices.size());
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	return 0;
}

std::vector<glm::vec3>* Quad3D::getVertices() {
	return &this->vertices;
}


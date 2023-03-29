#include "Camera.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

Camera::Camera()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	direction = glm::vec3(0.0f, 0.0f, 0.0f);
}
Camera::~Camera()
{
}
void Camera::setPosition(glm::vec3 position)
{
	this->position = position;
}
void Camera::setDirection(glm::vec3 direction)
{
	this->direction = direction;
}
void Camera::move(float magnitude, int direction) {
	switch (direction)
	{
	case FORWARD:
		this->position += glm::vec3(
			sin(this->direction.y)* -magnitude, 
			sin(this->direction.x)* magnitude, 
			cos(this->direction.y)* magnitude);
		break;
	case BACKWARD:
		this->position += glm::vec3(
			sin(this->direction.y) * magnitude,
			sin(this->direction.x) * -magnitude,
			cos(this->direction.y) * -magnitude);
		break;
	case LEFT:
		this->position += glm::vec3(
			cos(this->direction.y) * magnitude,
			0,
			sin(this->direction.y) * magnitude);
		break;
	case RIGHT:
		this->position += glm::vec3(
			cos(this->direction.y) * -magnitude,
			0,
			sin(this->direction.y) * -magnitude);
		break;
	default:
		break;
	}
}
void Camera::rotate(float magnitude, int direction)
{
	switch (direction)
	{
	case UP:
		this->direction += glm::vec3(-magnitude, 0.0f, 0.0f);
		break;
	case DOWN:
		this->direction += glm::vec3(magnitude, 0.0f, 0.0f);
		break;
	case LEFT:
		this->direction += glm::vec3(0.0f, -magnitude, 0.0f);
		break;
	case RIGHT:
		this->direction += glm::vec3(0.0f, magnitude, 0.0f);
		break;
	default:
		break;
	}
}
glm::vec3 Camera::getPosition()
{
	return this->position;
}
glm::vec3 Camera::getDirection()
{
	return this->direction;
}
glm::mat4 Camera::getMatrix(){
	glm::mat4 matrix = glm::mat4(1.0f);
	matrix = glm::rotate(matrix, direction.x, glm::vec3(1.0, 0.0, 0.0));
	matrix = glm::rotate(matrix, direction.y, glm::vec3(0.0, 1.0, 0.0));
	matrix = glm::rotate(matrix, direction.z, glm::vec3(0.0, 0.0, 1.0));
	matrix = glm::translate(matrix, position);
	return matrix;
}

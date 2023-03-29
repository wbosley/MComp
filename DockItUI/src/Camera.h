#pragma once
#include "glm/glm.hpp"
class Camera
{
	public:
		Camera();
		~Camera();
		void setPosition(glm::vec3 position);
		void setDirection(glm::vec3 direction);
		void move(float magnitude, int direction);
		void rotate(float magnitude, int direction);
		glm::vec3 getPosition();
		glm::vec3 getDirection();
		glm::mat4 getMatrix();
		
	private:
		glm::vec3 position;
		glm::vec3 direction;
#define FORWARD 0
#define BACKWARD 1
#define LEFT 2
#define RIGHT 3
#define UP 0
#define DOWN 1

};


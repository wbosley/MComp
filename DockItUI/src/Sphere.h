#pragma once
#include "Model3D.h"
#include <iostream>


class Sphere: public Model3D
{
public:
	Sphere();
	int constructGeometry(int level);
	void setRadius(float rad);
	void setCentre(glm::vec3 coord);
	void initSphere();

private:
	float cx, cy, cz; //centre of the sphere
	float r;		//radius of the sphere
};


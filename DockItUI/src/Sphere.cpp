#include "Sphere.h"


const double PI = 3.14159265358979323846f;


Sphere::Sphere()
{
	cx = cy = cz = 0.0;
	r = 0.5;
}

void Sphere::setRadius(float rad)
{
	r = rad;
}

void Sphere::setCentre(glm::vec3 coord)
{
	std::cout<<"vertex1 before: " << vertices[0].x << " " << vertices[0].y << " " << vertices[0].z << std::endl;
	
	//loop through vertices
	for (int i = 0; i < vertices.size(); i++) {
		vertices[i].x += coord.x;
		vertices[i].y += coord.y;
		vertices[i].z += coord.z;
	}

	std::cout << "vertex1 after: " << vertices[0].x << " " << vertices[0].y << " " << vertices[0].z << std::endl;
	updateBufferData();
}

int Sphere::constructGeometry(int level)
{
	//create the memory for the sphere
	int sizeOfVerts = ((level - 2) * level + 2) * 3;
	int sizeOfNorms = ((level - 2) * level + 2) * 3;
	int sizeOfTInds = (((level - 3) * (level - 1) + (level - 1)) * 2) * 3;

	float* verts = new float[sizeOfVerts];
	float* norms = new float[sizeOfNorms];
	unsigned int* tInds = new unsigned int[sizeOfTInds];

	int numOfTris = (((level - 3) * (level - 1) + (level - 1)) * 2);
	int numOfVerts = ((level - 2) * level + 2);

	//populate the arrays
	float theta, phi;
	int vertCount = 0;
	int i, j, t;

	for (t = 0, j = 1; j < level - 1; j++)
	{
		for (i = 0; i < level; i++)
		{
			theta = float(j) / (level - 1) * PI;
			phi = float(i) / (level - 1) * PI * 2;

			verts[vertCount + t] = ((sinf(theta) * cosf(phi)) * r) + cx;
			verts[vertCount + t + 1] = (cosf(theta) * r) + cy;
			verts[vertCount + t + 2] = ((-sinf(theta) * sinf(phi)) * r) + cz;

			norms[vertCount + t] = (sinf(theta) * cosf(phi));
			norms[vertCount + t + 1] = cosf(theta);
			norms[vertCount + t + 2] = -(sinf(theta) * sinf(phi));

			t += 3;
		}
	}
	verts[vertCount + t] = cx;
	verts[vertCount + t + 1] = r + cy;
	verts[vertCount + t + 2] = cz;

	norms[vertCount + t] = 0;
	norms[vertCount + t + 1] = 1;
	norms[vertCount + t + 2] = 0;
	t += 3;

	verts[vertCount + t] = cx;
	verts[vertCount + t + 1] = -r + cy;
	verts[vertCount + t + 2] = cz;

	norms[vertCount + t] = 0;
	norms[vertCount + t + 1] = -1;
	norms[vertCount + t + 2] = 0;
	t += 3;

	int vc3 = vertCount / 3;
	int triCount = 0;
	for (t = 0, j = 0; j < level - 3; j++)
	{
		for (i = 0; i < level - 1; i++)
		{
			tInds[triCount + t] = vc3 + ((j)*level + i);    t++;
			tInds[triCount + t] = vc3 + ((j + 1) * level + i + 1);    t++;
			tInds[triCount + t] = vc3 + ((j)*level + i + 1);    t++;
			tInds[triCount + t] = vc3 + ((j)*level + i);    t++;
			tInds[triCount + t] = vc3 + ((j + 1) * level + i);    t++;
			tInds[triCount + t] = vc3 + ((j + 1) * level + i + 1);  t++;
		}
	}
	for (i = 0; i < level - 1; i++)
	{
		tInds[triCount + t] = vc3 + ((level - 2) * level);  t++;
		tInds[triCount + t] = vc3 + (i);    t++;
		tInds[triCount + t] = vc3 + (i + 1);    t++;
		tInds[triCount + t] = vc3 + ((level - 2) * level + 1);    t++;
		tInds[triCount + t] = vc3 + ((level - 3) * level + i + 1);    t++;
		tInds[triCount + t] = vc3 + ((level - 3) * level + i);  t++;
	}
	triCount += t;

	for (i = 0; i < sizeOfVerts; i += 3)
	{
		vertices.push_back(glm::vec3(verts[i], verts[i + 1], verts[i + 2]));
	}

	for (i = 0; i < sizeOfNorms; i += 3)
	{
		normals.push_back(glm::vec3(norms[i], norms[i + 1], norms[i + 2]));
	}

	for (i = 0; i < sizeOfTInds; i++)
	{
		vertices_indices.push_back(tInds[i]);
	}

	int vaoAndVboinitialised = putModelDataInVbosAndVaos();
	//adding together all the returned values to make sure ntohing broke. if more than 0 then something went wrong.
	return 0 + vaoAndVboinitialised;
	return 0;
}
#shader vertex
#version 410

uniform mat4 matrix;

layout(location = 0) in vec4 position;
//layout(location = 1) in vec3 normal;
layout(location = 2) in vec4 colour;

out vec4 vColour;

void main()
{
	gl_Position = matrix * position;
	vColour = colour;
};

#shader fragment
#version 410 core

in vec4 vColour;

out vec4 outputColour;

void main()
{
	outputColour = vColour;
};
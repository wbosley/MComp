#shader vertex
#version 410

uniform mat4 matrix;

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;

void main()
{
	gl_Position = matrix * position;
};

#shader fragment
#version 410 core

layout(location = 0) out vec4 color;

void main()
{
	color = vec4(1.0, 1.0, 1.0, 1.0);
};
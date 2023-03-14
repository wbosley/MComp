#shader vertex
#version 410

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 v3NormalIn;

void main()
{
	gl_Position = position;
};

#shader fragment
#version 410 core

out vec4 outputColor;

void main()
{
	outputColor = vec4(1.0, 1.0, 1.0, 1.0);
};
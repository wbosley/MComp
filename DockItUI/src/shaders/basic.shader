#shader vertex
#version 410

uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;

void main()
{
	gl_Position = position * ModelViewMatrix * ProjectionMatrix;
};

#shader fragment
#version 410 core

layout(location = 0) out vec4 color;

void main()
{
	color = vec4(0.5, 0.0, 0.5, 1.0);
};
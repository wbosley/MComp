#shader vertex
#version 410

uniform mat4 matrix;

layout(location = 0) in vec4 position;
//layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCordin;
out vec2 texCord;

void main()
{
	texCord = texCordin;
	gl_Position = matrix * position;
}

#shader fragment
#version 410 core

in vec2 texCord;
uniform sampler2D DiffuseMap;

out vec4 color;

void main()
{
	color = texture(DiffuseMap, texCord);
	//color = vec4(1.0, 1.0, 1.0, 1.0);
}
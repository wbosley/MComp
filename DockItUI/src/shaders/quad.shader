#shader vertex
#version 410

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCordin;
out vec2 texCord;
void main()
{
	texCord = texCordin;
	gl_Position = vec4(position, 1.0);
}

#shader fragment
#version 410 core

in vec2 texCord;
uniform sampler2D DiffuseMap;

out vec4 color;

void main()
{
	color = texture(DiffuseMap, texCord);
}
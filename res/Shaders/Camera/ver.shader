#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aNormal;

out vec2 TexCoords;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

void main()
{
	TexCoords = aTexCoords;
	gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
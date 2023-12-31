#version 330 core

layout (location = 0) in vec4 aPos;
layout (location = 1) in vec3 aCol;

out vec3 ourColor;

void main()
{
	gl_Position = vec4(aPos.x, aPos.y, 1.0 , 1.0);
	ourColor = aCol;
}
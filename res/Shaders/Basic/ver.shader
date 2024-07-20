#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;

out vec3 ourColor;

uniform mat4 view  = mat4(
	1, 0, 0, 0,
	0,	1, 0, 0,
	0, 0,	1, 0,
	0, 0, 0, 1
);

uniform mat4 projection = mat4(
	1, 0, 0, 0,
	0,	1, 0, 0,
	0, 0,	1, 0,
	0, 0, 0, 1
);

uniform mat4 model = mat4(
	0.5, 0, 0, 0,
	0, 0.5, 0, 0,
	0, 0, 0.5, 0,
	0, 0, 0, 1
);

void main()
{
	gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	ourColor = aCol;
}
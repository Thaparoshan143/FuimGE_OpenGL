#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 3) in vec3 aNor;

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
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
);

void main()
{
	gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
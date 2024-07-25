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
	0.5, 0, 0, 0,
	0, 0.5, 0, 0,
	0, 0, 0.5, 0,
	0, 0, 0, 1
);

out vec3 aCol;

void main()
{
	vec4 temp = projection * view * model * vec4(aNor, 1.0);
	aCol = vec3(temp.x, temp.y, temp.z);
	gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
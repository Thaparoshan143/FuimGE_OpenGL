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

out vec3 FragPos;
out vec3 Normal;

void main()
{
	Normal = mat3(transpose(inverse(model))) * aNor;
	FragPos = vec3(model * vec4(aPos, 1.0));
	gl_Position = projection * view * vec4(FragPos, 1.0);
}
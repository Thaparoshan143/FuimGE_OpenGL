#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform vec3 col = vec3(1, 1, 1);

void main()
{
	FragColor = vec4(col.x, col.y, col.z, 1.0);
}
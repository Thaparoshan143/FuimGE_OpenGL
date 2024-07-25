#version 330 core

out vec4 FragColor;

in vec3 aCol;

uniform vec4 col;

void main()
{
	FragColor = vec4(aCol.x, aCol.y, aCol.z, 1.0);
}
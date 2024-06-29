// temporary
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

void main()
{    
    FragColor = vec4(TexCoords.x, TexCoords.y, TexCoords.x/(TexCoords.y*2.0), 1);
}
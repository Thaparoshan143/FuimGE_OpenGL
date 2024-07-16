#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light
{
	vec3 direction;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform vec3 veiwPos;
uniform vec3 col = vec3(1, 0.5, 0.2);
uniform float shininess = 32.0f;

uniform Light light;

void main()
{
	vec3 ambient = light.ambient * col;

	vec3 norm = normalize(Normal);

	vec3 lightDir = normalize(-light.direction);
	float diff_mag = max(dot(norm, lightDir), 0);
	vec3 diffuse = light.diffuse * diff_mag * col;

	vec3 viewDir = normalize(veiwPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	
	float spec_mag = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	vec3 specular = light.specular * spec_mag * col;

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
}
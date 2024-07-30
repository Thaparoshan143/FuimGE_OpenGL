#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

struct Material
{
	vec4 color;

	float ambient;
	float diffuse;
	float specular;
	float roughness;
};

struct Light
{
	vec3 direction;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform vec3 veiwPos;
uniform float shininess = 32.0f;

uniform Light light;
uniform Material material;

void main()
{
	vec3 col = vec3(material.color.x, material.color.y, material.color.z);
	vec3 ambient = light.ambient * material.ambient * col;

	vec3 norm = normalize(Normal);

	vec3 lightDir = normalize(-light.direction);
	float diff_mag = max(dot(norm, lightDir), 0);
	vec3 diffuse = light.diffuse * diff_mag * material.diffuse * col;

	vec3 viewDir = normalize(veiwPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	
	float spec_mag = pow(max(dot(viewDir, reflectDir), 0.0), shininess) * (1.15 - material.roughness);
	vec3 specular = light.specular * spec_mag * material.specular * col;

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, material.color.w);
}
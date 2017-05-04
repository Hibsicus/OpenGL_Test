#version 330 core
struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec2 TexCoords;

in vec3 Normal;
in vec3 FragPos;

out vec4 color;

uniform Light light;
uniform Material material;
uniform vec3 viewPos;

void main()
{
	//環境光
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	
	
	//漫反射光
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
	
	//鏡面高光
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

	color = vec4(ambient + diffuse + specular, 1.0f);	

/********************************************************************
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;
	
	float specularStrength = 0.5f;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength  * spec * lightColor;

	float ambientStrength = 0.1f;
	vec3 ambient = ambientStrength  * lightColor;
	vec3 result = (ambient + diffuse + specular) * objectColor;
	color = vec4(result, 1.0f);
**********************************************************************/
}
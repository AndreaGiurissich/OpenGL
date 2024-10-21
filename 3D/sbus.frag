#version 400 core
//output colori in RGBA
out vec4 FragColor;

in vec2 TexCoords;

in vec3 Normal;
in vec3 crntPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_height1;

uniform vec4 lightColor;
uniform vec3 lightPos;

uniform vec3 camPos;

float near = 0.1; 
float far  = 100.0; 

vec4 pointLight()
{

	vec3 lightVec = lightPos - crntPos;
	// variabili della point light
	float dist = length(lightVec);
	float a = 0.05f;
	float b = 0.01f;
	float inten = 1.0f / (a * dist * dist + b * dist + 1.0f);

	//ambient lighting
	float ambient = 0.20f;

	//diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightVec);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	//specular lighting 
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;

	return (texture(texture_diffuse1, TexCoords) * (diffuse * inten + ambient) + texture(texture_specular1, TexCoords).r * specular * inten) ;
}

vec4 direcLight()
{
	//ambient lighting
	float ambient = 0.20f;

	//diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(vec3(1.0f, 1.0f, 0.0f));
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	//specular lighting 
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;

	return (texture(texture_diffuse1, TexCoords) * (diffuse + ambient) + texture(texture_specular1, TexCoords).r * specular);
}

vec4 spotLight()
{
	//variabili per l'ampiezza del cono
	float outerCone = 0.90f;
	float innerCone = 0.95f;

	//ambient lighting 
	float ambient = 0.20f;

	//diffuse lighting 
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - crntPos);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	//specular lighting
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;

	float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
	float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

	return (texture(texture_diffuse1, TexCoords) * (diffuse * inten + ambient) + texture(texture_specular1, TexCoords).r * specular * inten) ;
}

float linearizeDepth(float depth)
{
	return (2.0 * near * far) / (far + near - (depth * 2.0 - 1.0) * (far - near));
}

void main()
{
	// outputs final color
	//FragColor = direcLight() * (1.0f - depth) + vec4(depth * vec3(0.85f, 0.85f, 0.90f), 1.0f);
	FragColor = direcLight();
}
#version 400 core
//output colori in RGBA
out vec4 FragColor;

in vec2 TexCoords;

in vec3 Normal;
in vec3 crntPos;

in vec4 fragPosLight;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D shadowMap;

uniform vec4 lightColor;
uniform vec3 lightDirection;
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

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
	if(projCoords.x < 0.0 || projCoords.x > 1.0 || 
       projCoords.y < 0.0 || projCoords.y > 1.0 ||
       projCoords.z < 0.0 || projCoords.z > 1.0)
    {
        return 0.0; // Not in shadow if outside shadow map
    }
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;

	// PCF with 3x3 kernel
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += (currentDepth - 0.01) > pcfDepth ? 1.0 : 0.0;        
        }    
    }
	return shadow /= 9.0;
}  

vec4 direcLight()
{
	//ambient lighting
	float ambient = 0.20f;

	//diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightDirection);
	vec3 lightDirectionMir = vec3(lightDirection.x, -lightDirection.y, lightDirection.z);
	float diffuse = max(dot(normal, lightDirectionMir), 0.0f);

	//specular lighting 
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - crntPos);
	lightDirectionMir = vec3(-lightDirection.x, lightDirection.y, -lightDirection.z);
	vec3 reflectionDirection = reflect(lightDirectionMir, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;

	return (texture(texture_diffuse1, TexCoords) * (diffuse * (1.0f - ShadowCalculation(fragPosLight)) + ambient) + texture(texture_specular1, TexCoords).r * specular * (1.0f - ShadowCalculation(fragPosLight)));
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
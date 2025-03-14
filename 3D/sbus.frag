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

uniform vec3 flamePos1;
uniform vec3 flamePos2;
uniform vec3 flamePos3;
uniform vec3 flamePos4;
uniform vec4 flameLightColor;

uniform float angle;

uniform vec3 camPos;

float near = 0.1; 
float far  = 100.0; 

// Calculate light color based on angle
vec4 calculateLightColor()
{
    vec4 computedLightColor; // Default white light
    vec4 lastcomputedLightColor;

    if (angle >= 0.0 && angle <= 45.0) {
        float t = (angle - 0.0) / 45.0; // Normalize angle to range [0, 1]
        computedLightColor = vec4(1.0, 1.0 - 0.6 * t, 1.0 - t, 1.0); // White to red
    }
    else if (angle >= 230.0 && angle <= 275.0) {
        float t = (angle - 230.0) / 45.0; // Normalize angle to range [0, 1]
        computedLightColor = vec4(1.0, 0.6 + 0.4 * t, 0.0 + t, 1.0); // Red to white
    }
	else if (angle >= 45.0f && angle <= 230.0f) 
	{
		vec4 nightColor = vec4(0.2, 0.3, 0.6, 1.0);
		vec4 finalLightColor;

		float alpha;

			if (angle <= 137.5f) 
			{
				// Prima metà notte: 45° to 137.5°
				alpha = (angle - 45.0f) / 92.5f; // Normalized to 0-1
				finalLightColor = mix(vec4(1.0, 0.4, 0.0, 1.0), nightColor, alpha);
				return finalLightColor;
			}
			else 
			{
				// Seconda metà notte: 137.5° to 230°
				alpha = (230.0f - angle) / 92.5f; // Normalized to 1-0
				finalLightColor = mix(vec4(1.0, 0.7, 0.0, 1.0), nightColor, alpha);
				return finalLightColor;
			}


	}else computedLightColor = vec4(1.0, 1.0, 1.0, 1.0); // Default white light

    return computedLightColor;
}

vec4 pointLight(vec3 flameLightPos) {
    vec3 lightVec = flameLightPos - crntPos;
    float dist = length(lightVec);
    float attenuation = 0.2f;

    // Diffuse
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(lightVec);
    float diffuse = max(dot(normal, lightDir), 0.0);

    // Specular
    vec3 viewDir = normalize(camPos - crntPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.5), 32.0);
    float specular = spec * 0.5;

    return (texture(texture_diffuse1, TexCoords) * diffuse * attenuation + 
            texture(texture_specular1, TexCoords).r * specular * attenuation) * flameLightColor;
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
	float ambient = 0.30f;

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

	vec4 finalLightColor = calculateLightColor();

	//vec4 finalLightColor = lightColor;

	return (texture(texture_diffuse1, TexCoords) * (diffuse * (1.0f - ShadowCalculation(fragPosLight)) + ambient) + texture(texture_specular1, TexCoords).r * specular * (1.0f - ShadowCalculation(fragPosLight))) * finalLightColor;
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
	vec4 directionalLight = direcLight();
	vec4 flameLight1 = pointLight(flamePos1);
	vec4 flameLight2 = pointLight(flamePos2);
	vec4 flameLight3 = pointLight(flamePos3);
	vec4 flameLight4 = pointLight(flamePos4);

	if(angle >=60.0 && angle <= 215.0)
	{
		FragColor = directionalLight + flameLight1 + flameLight2 + flameLight3 + flameLight4;
	}
	else
	{
		FragColor = directionalLight;
	}
}
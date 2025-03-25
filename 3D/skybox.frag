#version 400 core
out vec4 FragColor;

in vec3 texCoords;

uniform samplerCube skybox;
uniform float angle;

float computeAlpha()
{
    float alpha;

	//giorno - tramonto
	if (angle >= 5.0f && angle < 60.0f) 
	{
		if (angle <= 25.0f) 
		{
			// giorno - tramonto: 5° a 25°
			alpha = (angle - 5.0f) / 40.0f; // Normalized to 0-0.5
			return alpha;
		}
		else if (angle > 25.0f && angle <= 60.0f)
		{
			// tramonto: 25° to 60°
			alpha = 0.5f;
			return alpha;
		}
	}

	//tramonto - notte
    if (angle >= 60.0f && angle <= 185.0f) 
	{
		if (angle <= 110.0f) 
		{
			// tramonto - notte: 60° to 110°
			alpha = (angle - 60.0f) / 50.0f; // Normalized to 0-1 
			return alpha;
		}
		else if (angle > 110.0f && angle <= 185.0f)
		{
			// notte: 110° to 185°
			alpha = 1.0f;
			return alpha;
		}
	}

	//notte - alba
	if(angle > 185.0f && angle <= 245.0f)
	{
		if(angle <= 245.0f)
		{
			// notte - alba: 185° to 225°
			alpha = (angle - 185.0f) / 60.0f; // Normalized to 0-1 
			return alpha;
		}
		
	}

	//alba - giorno
	if(angle > 245.0f && angle <= 300.0f)
	{
		alpha = 0.5f + (angle - 245.0f) / 55.0f; // Normalized to 0.5-1
		return alpha;
	}

}

void main()
{    
	vec4 sunsetColor = vec4(0.8, 0.3, 0.0, 1.0);
	vec4 nightColor = vec4(0.0, 0.0, 0.1, 1.0);
	vec4 sunriseColor = vec4(0.6, 0.4, 0.0, 1.0);
	vec4 texture1 = texture(skybox, texCoords);

	if(angle < 60.0f)
	{
		FragColor = mix(texture1, sunsetColor, computeAlpha());
	}else if(angle >= 60.0f && angle <= 185.0f)
	{
		FragColor = mix(mix(texture1, sunsetColor, 0.5f), nightColor, computeAlpha());
	}else if(angle >= 185.0f && angle <= 245.0f)
	{
		FragColor = mix(nightColor, mix(texture1, sunriseColor, 0.5f), computeAlpha());
	}else if(angle > 245.0f && angle <= 300.0f)
	{
		FragColor = mix(sunriseColor, texture1, computeAlpha());
	}else
	{
		FragColor = texture1;
	}
}
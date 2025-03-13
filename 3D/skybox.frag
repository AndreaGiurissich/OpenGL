#version 400 core
out vec4 FragColor;

in vec3 texCoords;

uniform samplerCube skybox;
uniform float angle;

float computeAlpha()
{
    float alpha;

    if (angle >= 30.0f && angle <= 245.0f) 
	{
			if (angle <= 90.0f) 
			{
				// Prima metà notte: 45° to 137.5°
				alpha = (angle - 30.0f) / 60.0f; // Normalized to 0-1
				return alpha;
			}
			else if (angle > 90.0f && angle <= 185.0f)
			{
				// Notte: 90° to 185°
				alpha = 1.0f;
				return alpha;
			}else
			{	// Seconda metà notte: 185° to 245°
				alpha = (245.0f - angle) / 60.0f; // Normalized to 1-0
				return alpha;
			}
		}else
		return alpha = 0.0;

}


void main()
{    
	vec4 nightColor = vec4(0.0, 0.0, 0.1, 1.0);
	vec4 texture1 = texture(skybox, texCoords);

    FragColor = mix(texture1, nightColor, computeAlpha());
}
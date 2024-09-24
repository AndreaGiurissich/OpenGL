#version 400 core

out vec4 FragColor;

in vec3 color;
in vec2 texCoord;

uniform sampler2D texture1;
uniform int numSquares;

void main()
{
	FragColor = texture(texture1, texCoord*numSquares) + vec4(color, 1.0f);
 };
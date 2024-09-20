#version 400 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform int numSquares;

void main()
{
 FragColor = texture(texture1, TexCoord * 0.5f * numSquares) * vec4(ourColor, 1.0);
 };
#version 400 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 mvp;

out vec3 ourColor;
out vec2 TexCoord;

uniform mat4 cameraMatrix;

//uniform mat4 model;
//uniform mat4 view;
//uniform mat4 proj;

void main()
{
	gl_Position = cameraMatrix * vec4(aPos, 1.0);	
	ourColor = aColor;
	TexCoord = aTexCoord;
};
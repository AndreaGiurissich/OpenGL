#version 400 core

//positions/coordinates
layout (location = 0) in vec3 aPos;
// Texture Coordinates
layout (location = 2) in vec2 aTexCoords;
//normal coordinates
layout (location = 1) in vec3 aNormal;

// Outputs the texture coordinates to the fragment shader
out vec2 TexCoords;

out vec3 Normal;
out vec3 crntPos;


uniform mat4 camMatrix;
uniform mat4 model;


void main()
{
	crntPos = vec3(model * vec4(aPos, 1.0f));

	
	//gl_Position = matrix * model * vec4(aPos, 1.0);
	//assegna colori da vertex data a color
	// Assigns the texture coordinates from the Vertex Data to "texCoord"
	TexCoords = aTexCoords; 

	Normal = aNormal;
	gl_Position = camMatrix * vec4(crntPos, 1.0);
}

#pragma once
#include <string>

#include "GLCommon.h"

class Skybox
{
	private:
	//Cubemap

	float skyboxVertices[24] = {
		// Posizioni          
		-1.0f,  -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, 1.0f,
		1.0f,  1.0f, 1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f
	};

	int skyboxIndices[36] = {
		// Destra
			1, 2, 6,
			6, 5, 1,
			// Sinistra
			0, 4, 7,
			7, 3, 0,
			// Sopra
			4, 5, 6,
			6, 7, 4,
			// Sotto
			0, 3, 2,
			2, 1, 0,
			// Dietro
			0, 1, 5,
			5, 4, 0,
			// Di fronte
			3, 7, 6,
			6, 2, 3

	};

	//std::string facesCubemap[6];

	unsigned int skyboxVAO, skyboxVBO, skyboxEBO;

	unsigned int cubemapTexture;

	public:
	
	Skybox(std::string facesCubemap[6]);
	void BindVAO();
};

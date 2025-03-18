#pragma once

#include "VAO.h"
#include "EBO.h"
#include "Textures.h"

struct Texture
{
	GLuint ID;
	std::string type;
	std::string path;
};

class Mesh
{
	public:
		std::vector <Vertex> vertices;
		std::vector <GLuint> indices;
		std::vector <Texture> textures;
		// Memorizza VAO a public così che può essere usato nella funzione Draw
		VAO VAO;
		
		// Inizializzare le mesh
		Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture>& textures);
		// Disegna le mesh
		void Draw(Shader& shader);

	private:
		void setupMesh();
		
		
};

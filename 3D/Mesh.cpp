#include "Mesh.h"


Mesh::Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture>& textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	setupMesh();
	
}

void Mesh::setupMesh()
{
	
	

	VAO.Bind();
	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO(vertices);
	// Generates Element Buffer Object and links it to indices
	EBO EBO(indices);
	// Links VBO attributes such as coordinates and colors to VAO
	VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	VAO.LinkAttrib(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
	VAO.LinkAttrib(VBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO.Unbind();
	VBO.Unbind();
	EBO.Unbind();
}

void Mesh::Draw(Shader& shader)
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;


	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		string number;
		string name = textures[i].type;

		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);
		else if (name == "texture_normal")
			number = std::to_string(normalNr++);

		int TextUniformLoc = glGetUniformLocation(shader.ID, (name + number).c_str());

		glUniform1i(TextUniformLoc, i);
		glBindTexture(GL_TEXTURE_2D, textures[i].ID);
	}


	// disegno mesh
	VAO.Bind();
	glDrawElements(GL_TRIANGLES, static_cast<GLuint>(indices.size()), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	//reset texture attiva
	glActiveTexture(GL_TEXTURE0);
}


//
//void Mesh::Draw(Shader& shader)
//{
//	shader.UseProgram();
//	VAO.Bind();
//
//	unsigned int numDiffuse = 1;
//	unsigned int numSpecular = 1;
//
//	for(unsigned int i = 0; i < textures.size(); i++)
//	{
//		std::string num;
//		std::string type = textures[i].type;
//		if(type == "diffuse")
//		{
//			num = std::to_string(numDiffuse++);
//		}
//		else if(type == "specular")
//		{
//			num = std::to_string(numSpecular++);
//		}
//		int TextUniformLoc = glGetUniformLocation(shader.ID, (type + num).c_str());
//
//		glUniform1i(TextUniformLoc, i);
//		glBindTexture(GL_TEXTURE_2D, textures[i].ID);
//	}
//	
//
//	// Draw the actual mesh
//	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
//
//}

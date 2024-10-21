#include"Textures.h"

#include <iostream>

#include "stb_image.h"

Textures::Textures(const char* image, GLenum texType, GLuint slot, GLenum format, GLenum pixelType)
{
	// tipo texture
	type = texType;

	// oggetto Opengl texture
	glGenTextures(1, &ID);
	// assegna la texture a Texture Unit
	glActiveTexture(GL_TEXTURE0 + slot);
	unit = slot;
	glBindTexture(texType, ID);

	// tipo di algoritmo per dimensione immagine
	glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// come ripete immagine
	glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Per GL_CLAMP_TO_BORDER (colore bordo)
	// float flatColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
	// glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColor);

	// altezza larghezza e numero di canali colore
	int widthImg, heightImg, numCh;
	// rigira immagine
	stbi_set_flip_vertically_on_load(true);
	// legge file e mette in data
	unsigned char* data = stbi_load(image, &widthImg, &heightImg, &numCh, 0);
	if (data)
	{
		// assegna img
		glTexImage2D(texType, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, data);
		// genera mipmap
		glGenerateMipmap(texType);
	}
	else
	{
		std::cout << "Errore caricamento texture" << std::endl;
	}

	// unbind per evitare modifiche
	glBindTexture(texType, 0);

	// elimina immagine caricata
	stbi_image_free(data);


}

void Textures::texUnit(Shader& shader, const char* uniform, GLuint unit)
{
	// prima del cambio valore dell'uniform attivare shader
	shader.UseProgram();
	// Sets the value of the uniform
	glUniform1i(glGetUniformLocation(shader.ID, uniform), unit);
}

void Textures::Bind()
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(type, ID);
}

void Textures::Unbind()
{
	glBindTexture(type, 0);
}

void Textures::Delete()
{
	glDeleteTextures(1, &ID);
}
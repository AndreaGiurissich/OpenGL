#include "gl.h"
#include "functions.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "stb_image.h"



extern unsigned int BuildTexture2D(std::string filepath, GLint wrapS_p, GLint wrapT_p, GLint Min_p, GLint Mag_p, GLenum format)
{
	unsigned int texture;
	int width, height, nrChannels;

	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &nrChannels, 0);
	
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS_p);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT_p);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Min_p);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Mag_p);

	if (!data)
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	else
	{
			

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
	}

	stbi_image_free(data);

	return texture;
	}

#pragma once
#include "GLCommon.h"
#include "shaderClass.h"


class Textures
{
public:
	GLuint ID;
	GLenum type;
	GLuint unit;
	

	Textures(const char * image, GLenum texType, GLuint slot, GLenum format, GLenum pixelType);

	void texUnit(Shader& program, const char* uniform, GLenum unit);

	void Bind();
	void Unbind();
	void Delete();

};


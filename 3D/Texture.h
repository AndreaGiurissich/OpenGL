#pragma once
#include "GLCommon.h"
#include "shaderClass.h"


class Texture
{
public:
	GLuint ID;
	GLuint unit;
	const char* type;

	Texture(std::string filepath, const char* type, GLint wrapS_p, GLint wrapT_p, GLint Min_p, GLint Mag_p, GLenum format, GLuint slot);

	void texUnit(Shader& program, const char* uniform, GLenum unit);

	void Bind();
	void Unbind();
	void Delete();

};


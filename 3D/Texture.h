#pragma once
#include "GLCommon.h"
#include "shaderClass.h"


class Texture
{
public:
	GLuint ID;

	Texture(std::string filepath, GLint wrapS_p, GLint wrapT_p, GLint Min_p, GLint Mag_p, GLenum format);

	void texUnit(Shader& program, const char* uniform, GLenum unit);

	void Bind();
	void Unbind();
	void Delete();

};


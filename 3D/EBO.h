#pragma once
#include "GLCommon.h"
#include <vector>


class EBO
{
	public:
		GLuint ID;

		EBO(std::vector<GLuint>& indices);
		//EBO(GLuint* indices, GLsizeiptr size);
		void Bind();
		void Unbind();
		void Delete();
};

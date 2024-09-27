#pragma once
#include <vector>

#include "GLCommon.h"
class EBO
{
	public:
		GLuint ID;

		EBO(std::vector<GLuint>& indices);
		void Bind();
		void Unbind();
		void Delete();
};

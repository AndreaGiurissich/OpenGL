#pragma once
#include <Algebra_Lineare.h>
#include <vector>

#include "GLCommon.h"
#include "Structs.h"


class VBO
{
	public:
		GLuint ID;

		VBO(vector <Vertex>& vertices);
		void Bind();
		void Unbind();
		void Delete();
};

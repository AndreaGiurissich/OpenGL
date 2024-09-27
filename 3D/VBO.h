#pragma once
#include <Algebra_Lineare.h>
#include <vector>

#include "GLCommon.h"


struct Vertex
{
	Vec3 position;
	Vec3 normal;
	Vec3 color;
	Vec2 texUV;
};

class VBO
{
	public:
		GLuint ID;

		VBO(vector <Vertex>& vertices);
		void Bind();
		void Unbind();
		void Delete();
};

#pragma once
#include "GLCommon.h"
#include <Algebra_Lineare.h>
#include <vector>

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

		// Constructor that generates a Vertex Buffer Object and links it to vertices
		VBO(vector<Vertex>& vertices);
		
		void Bind();
		void Unbind();
		void Delete();
};

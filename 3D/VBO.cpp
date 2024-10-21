#include "VBO.h"

VBO::VBO(vector <Vertex>& vertices)
{
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID); //collega il buffer, array buffer è il tipo per vertex
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
}


void VBO::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, ID); //Collega il buffer al target
}

void VBO::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0); //Collega il buffer al target
}

void VBO::Delete()
{
	glDeleteBuffers(1, &ID);
}

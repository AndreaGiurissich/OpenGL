#include "VBO.h"

VBO::VBO(GLfloat* vertices, GLsizeiptr size)
{
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID); //Collega il buffer al target
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW); //Carica i dati nel buffer

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

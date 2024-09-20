#include "VAO.h"

VAO::VAO()
{
	glGenVertexArrays(1, &ID);
}


void VAO::LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset)
{
	VBO.Bind();
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	//Abilita i vertex attribute cos� opengl sa come usarli
	glEnableVertexAttribArray(layout);
	VBO.Unbind();
}


void VAO::Bind()
{
	glBindVertexArray(ID);
}

void VAO::Unbind()
{
	glBindVertexArray(0);
}

void VAO::Delete()
{
	glDeleteVertexArrays(1, &ID);
}

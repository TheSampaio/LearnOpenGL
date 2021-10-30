#include"VAO.h"

VAO::VAO() // Constructor that generates a VAO ID
{
	glGenVertexArrays(1, &ID);
}

void VAO::LinkVBO(VBO& VBO, GLuint layout) // Links a VBO to the VAO using a certain layout
{
	VBO.Bind();
	glVertexAttribPointer(layout, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(layout);
	VBO.Unbind();
}

void VAO::Bind() // Binds the VAO
{
	glBindVertexArray(ID);
}

void VAO::Unbind() // Unbinds the VAO
{
	glBindVertexArray(0);
}

void VAO::Delete() // Deletes the VAO
{
	glDeleteVertexArrays(1, &ID);
}
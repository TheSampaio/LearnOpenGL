#include"EBO.h"

EBO::EBO(std::vector<GLuint>& indices) // Constructor that generates a Elements Buffer Object and links it to indices
{
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
}

void EBO::Bind() // Binds the EBO
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void EBO::Unbind() // Unbinds the EBO
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::Delete() // Deletes the EBO
{
	glDeleteBuffers(1, &ID);
}
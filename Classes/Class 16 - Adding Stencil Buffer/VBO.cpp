#include"VBO.h"

VBO::VBO(std::vector<Vertex>& vertices) // Constructor that generates a Vertex Buffer Object and links it to vertices
{
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
}

void VBO::Bind() // Binds the VBO
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::Unbind() // Unbinds the VBO
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::Delete() // Deletes the VBO
{
	glDeleteBuffers(1, &ID);
}
#include "PCH.h"
#include "EBO.h"

EBO::EBO(std::vector<GLuint> indices)
{
	// Creates and setup an element buffer object
	glGenBuffers(1, &m_Id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
}

EBO::~EBO()
{
	// Deletes the created element buffer
	glDeleteBuffers(1, &m_Id);
}

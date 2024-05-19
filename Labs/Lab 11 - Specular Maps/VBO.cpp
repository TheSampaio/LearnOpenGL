#include "PCH.h"
#include "VBO.h"

VBO::VBO(std::vector<GLfloat> vertices)
{
	// Creates and setup a vertex buffer object
	glGenBuffers(1, &m_Id);
	glBindBuffer(GL_ARRAY_BUFFER, m_Id);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
}

VBO::~VBO()
{
	// Deletes the created vertex array object from memory
	glDeleteBuffers(1, &m_Id);
}

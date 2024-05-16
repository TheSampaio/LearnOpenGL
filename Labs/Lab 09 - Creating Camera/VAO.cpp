#include "PCH.h"
#include "VAO.h"

VAO::VAO()
{
	// Create and activates a vertex array object
	glGenVertexArrays(1, &m_Id);
	glBindVertexArray(m_Id);
}

VAO::~VAO()
{
	// Deletes the created vertex array object from memory
	glDeleteVertexArrays(1, &m_Id);
}

void VAO::AttribPointer(GLuint layout, GLint size, GLsizei stride, GLubyte offset, GLenum type, GLboolean normalized)
{
	glVertexAttribPointer(layout, size, type, normalized, stride, reinterpret_cast<void*>(static_cast<long long>(offset)));
	glEnableVertexAttribArray(layout);
}

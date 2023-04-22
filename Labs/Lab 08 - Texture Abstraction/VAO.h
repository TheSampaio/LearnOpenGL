#ifndef LEARNOPENGL_VAO_H
#define LEARNOPENGL_VAO_H

class VAO
{
public:
	VAO();
	~VAO();

	// Set-ups the VAO's layouts
	void AttribPointer(GLuint Layout, GLint Size, GLsizei Stride, GLenum Offset, GLenum Type = GL_FLOAT, GLboolean Normalized = GL_FALSE);

	inline void Bind() { glBindVertexArray(m_Id); }
	inline void Unbind() { glBindVertexArray(0); }

private:
	// Attributes
	GLuint m_Id;
};


#endif
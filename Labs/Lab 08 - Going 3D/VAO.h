#ifndef LEARNOPENGL_VAO_H
#define LEARNOPENGL_VAO_H

class VAO
{
public:
	VAO();
	~VAO();

	// Activates and set-ups the VAO's layouts
	void AttribPointer(GLuint layout, GLint size, GLsizei stride, GLubyte offset, GLenum type = GL_FLOAT, GLboolean normalized = GL_FALSE);

	// Binds the VAO
	inline void Bind() { glBindVertexArray(m_Id); }

	// Unbinds the VAO
	inline void Unbind() { glBindVertexArray(0); }

private:
	// Attributes
	GLuint m_Id;
};

#endif
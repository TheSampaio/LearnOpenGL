#ifndef LEARNOPENGL_EBO_H
#define LEARNOPENGL_EBO_H

class EBO
{
public:
	EBO(std::vector<GLuint> Indices);
	~EBO();

	// Main methods
	inline void Bind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id); }
	inline void Unbind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

private:
	// Attributes
	GLuint m_Id;
};

#endif

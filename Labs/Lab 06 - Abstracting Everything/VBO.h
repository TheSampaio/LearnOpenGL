#ifndef LEARNOPENGL_VBO_H
#define LEARNOPENGL_VBO_H

class VBO
{
public:
	VBO(std::vector<GLfloat> vertices);
	~VBO();

	// Binds the VBO
	inline void Bind() { glBindBuffer(GL_ARRAY_BUFFER, m_Id);}

	// Unbinds the VBO
	inline void Unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }

private:
	// Attributes
	GLuint m_Id;
};

#endif
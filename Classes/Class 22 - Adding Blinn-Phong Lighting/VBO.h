#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include<glad/glad.h>

class VBO
{
public:	
	GLuint ID;									// Reference ID of the Vertex Buffer Object
	VBO(GLfloat* vertices, GLsizeiptr size);	// Constructor that generates a Vertex Buffer Object and links it to vertices
	
	void Bind();	// Binds the VBO
	void Unbind();	// Unbinds the VBO
	void Delete();	// Deletes the VBO
};

#endif
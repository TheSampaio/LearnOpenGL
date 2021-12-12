#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include<glad/glad.h>
#include"VBO.h"

class VAO
{
public:

	GLuint ID;	// ID reference for the Vertex Array Object
	VAO();		// Constructor that generates a VAO ID

	void LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset); // Links a VBO to the VAO using a certain layout
	void Bind();	// Binds the VAO
	void Unbind();	// Unbinds the VAO
	void Delete();	// Deletes the VAO
};
#endif
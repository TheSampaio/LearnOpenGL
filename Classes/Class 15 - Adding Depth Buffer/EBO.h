#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include <glad/glad.h>
#include <vector>

class EBO
{
public:
	
	GLuint ID;							// ID reference of Elements Buffer Object
	EBO(std::vector<GLuint>& indices);	// Constructor that generates a Elements Buffer Object and links it to indices

	
	void Bind();	// Binds the EBO
	void Unbind();	// Unbinds the EBO
	void Delete();	// Deletes the EBO
};

#endif
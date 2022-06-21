#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <vector>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 texUV;
};

class VBO
{
public:
	
	GLuint ID;									// Reference ID of the Vertex Buffer Object
	VBO(std::vector<Vertex>& vertices);	// Constructor that generates a Vertex Buffer Object and links it to vertices
	
	void Bind();	// Binds the VBO
	void Unbind();	// Unbinds the VBO
	void Delete();	// Deletes the VBO
};

#endif
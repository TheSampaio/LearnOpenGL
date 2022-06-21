#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include <string>

#include "VAO.h"
#include "EBO.h"
#include "Camera.h"
#include "Texture.h"

class Mesh
{
public:
	Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures);

	VAO VAO; // Create a Vertex Array Object

	// Store all data that we need for a mesh
	std::vector<Vertex>  vertices;
	std::vector<GLuint>   indices;
	std::vector<Texture> textures;
	
	// Draw call function
	void Draw(Shader& shader, Camera& camera);
};
#endif
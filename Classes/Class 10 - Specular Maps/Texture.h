#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include <glad/glad.h>
#include <stb/stb_image.h>

#include "ShaderClass.h"

class Texture
{
public:
	GLuint ID;	 // ID reference for the Texture
	GLenum type; // The texture's type (1D, 2D or 3D)
	GLuint unit;

	Texture(const char* image, GLenum texType, GLuint slot, GLenum format, GLenum pixelType); // Texture class constructor

	void texUnit(Shader& shader, const char* uniform, GLuint unit); // Function that will receive the texture uniform from Fragment Shader

	void Bind();	// Binds the texture
	void Unbind();	// Unbinds the texture
	void Delete();	// Delete the texture
};
#endif // !TEXTURE_CLASS_H

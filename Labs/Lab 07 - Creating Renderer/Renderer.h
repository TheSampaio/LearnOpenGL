#ifndef LEARNOPENGL_RENDERER_H
#define LEARNOPENGL_RENDERER_H

#include "Texture.h"
#include "Shader.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

class Renderer
{
public:
	// Returns a reference for the renderer
	static inline Renderer& GetInstance() { static Renderer s_Instance; return s_Instance; }

	// Draw call command
	inline void Draw(std::vector<GLuint> indices, GLenum primitive = GL_TRIANGLES) { glDrawElements(primitive, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, nullptr); }

	// Set-ups an uniform 1 integer
	inline void SetUniform1i(Shader& shader, const GLchar* name, GLint value) { glUniform1i(glGetUniformLocation(shader.GetId(), name), value); }

private:
	Renderer() {};

	// Deletes copy contructor and assigment operator
	Renderer(const Renderer&) = delete;
	Renderer operator=(const Renderer&) = delete;
};

#endif // !LEARNOPENGL_RENDERER_H

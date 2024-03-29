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
	static inline Renderer& GetInstance() { static Renderer m_Instance; return m_Instance; }

	// Draw call command
	inline void Draw(std::vector<GLuint> Indices, GLenum Primitive = GL_TRIANGLES) { glDrawElements(Primitive, static_cast<GLsizei>(Indices.size()), GL_UNSIGNED_INT, nullptr); }

	// Set-ups an uniform 1 integer
	inline void SetUniform1i(Shader& ShaderProgram, const GLchar* Name, GLint Integer) { glUniform1i(glGetUniformLocation(ShaderProgram.GetId(), Name), Integer); }

private:
	Renderer() {};

	// Deletes copy contructor and assigment operator
	Renderer(const Renderer&) = delete;
	Renderer operator=(const Renderer&) = delete;
};

#endif // !LEARNOPENGL_RENDERER_H

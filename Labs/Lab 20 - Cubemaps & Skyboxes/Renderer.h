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
	inline void Draw(std::vector<GLuint> indices, GLenum primitive = GL_TRIANGLES)
	{ glDrawElements(primitive, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, nullptr); }

	inline void SetFaceCulling(bool enable)
	{ glEnable(GL_CULL_FACE); glCullFace(GL_FRONT); glFrontFace(GL_CCW); }

	// Set-ups an uniform 1 integer
	inline void SetUniform1i(Shader& shader, const GLchar* name, GLint value)
	{ glUniform1i(glGetUniformLocation(shader.GetId(), name), value); }

	// Set-ups an uniform 1 float
	inline void SetUniform1f(Shader& shader, const GLchar* name, GLfloat value)
	{ glUniform1f(glGetUniformLocation(shader.GetId(), name), value); }

	// Set-ups an uniform 3 float
	inline void SetUniform3f(Shader& shader, const GLchar* name, GLfloat value1, GLfloat value2, GLfloat value3)
	{ glUniform3f(glGetUniformLocation(shader.GetId(), name), value1, value2, value3); }

	// Set-ups an uniform matrix 4 float vector
	inline void SetUniformMatrix4fv(Shader& shader, const GLchar* name, glm::mat4 value)
	{ glUniformMatrix4fv(glGetUniformLocation(shader.GetId(), name), 1, GL_FALSE, glm::value_ptr(value)); }

private:
	Renderer() {};

	// Deletes copy contructor and assigment operator
	Renderer(const Renderer&) = delete;
	Renderer operator=(const Renderer&) = delete;
};

#endif // !LEARNOPENGL_RENDERER_H

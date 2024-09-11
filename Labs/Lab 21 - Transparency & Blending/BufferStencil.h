#ifndef LEARNOPENGL_STENCILBUFFER_H
#define LEARNOPENGL_STENCILBUFFER_H

class Camera;
class Shader;

class BufferStencil
{
public:
	BufferStencil(GLenum stencilFail, GLenum depthFail, GLenum depthPass);

	void Begin(GLenum function, GLint reference, GLuint mask = 0xFF);
	void End(GLenum function, GLint reference, GLuint mask = 0xFF);
	void Use(Shader& shader, Camera& camera, GLenum function = GL_NOTEQUAL, GLint reference = 1, GLuint mask = 0xFF);
};

#endif // !LEARNOPENGL_STENCILBUFFER_H

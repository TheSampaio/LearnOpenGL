#include "PCH.h"
#include "BufferStencil.h"

#include "Camera.h"
#include "Renderer.h"

BufferStencil::BufferStencil(GLenum stencilFail, GLenum depthFail, GLenum depthPass)
{
	// Enable OpenGL stencil buffer
	glEnable(GL_STENCIL_TEST);
	glStencilOp(stencilFail, depthFail, depthPass);
}

void BufferStencil::Begin(GLenum function, GLint reference, GLuint mask)
{
	glStencilFunc(function, reference, mask);
	glStencilMask(mask);
}

void BufferStencil::End(GLenum function, GLint reference, GLuint mask)
{
	glStencilMask(0xFF);
	glStencilFunc(GL_ALWAYS, 0, 0xFF);
}

void BufferStencil::Use(Shader& shader, Camera& camera, GLenum function, GLint reference, GLuint mask)
{
	glStencilFunc(function, reference, mask);
	glStencilMask(0x00);
	glDisable(GL_DEPTH_TEST);

	// Binds outline shader program
	shader.Bind();

	// Send the outline amount to the GPU
	Renderer::GetInstance().SetUniform1f(shader, "uOutline", 1.008f);

	// Recalculates camera matrix
	camera.Use(shader);
}

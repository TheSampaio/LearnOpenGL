#include "PCH.h"
#include "Renderer.h"

void Renderer::SetFaceCulling(bool enable)
{
	if (enable)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		glFrontFace(GL_CCW);
	}

	else
		glDisable(GL_CULL_FACE);
}

void Renderer::SetBlending(bool enable, GLenum sFactor, GLenum dFactor)
{
	if (enable)
	{
		glEnable(GL_BLEND);
		glBlendFunc(sFactor, dFactor);
	}

	else
		glDisable(GL_BLEND);
}

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

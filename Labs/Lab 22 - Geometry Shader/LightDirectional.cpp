#include "PCH.h"
#include "LightDirectional.h"

#include "Renderer.h"
#include "Shader.h"

LightDirectional::LightDirectional(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
{
	m_Direction = direction;
	m_Ambient = ambient;
	m_Diffuse = diffuse;
	m_Specular = specular;
}

void LightDirectional::Use(Shader& shader) const
{
	Renderer& renderer = Renderer::GetInstance();
	shader.Bind();

	renderer.SetUniform3f(shader, "uLightDirectional.base.ambient", m_Ambient.r, m_Ambient.g, m_Ambient.b);
	renderer.SetUniform3f(shader, "uLightDirectional.base.diffuse", m_Diffuse.r, m_Diffuse.g, m_Diffuse.b);
	renderer.SetUniform3f(shader, "uLightDirectional.base.specular", m_Specular.r, m_Specular.g, m_Specular.b);
	renderer.SetUniform3f(shader, "uLightDirectional.direction", m_Direction.x, m_Direction.y, m_Direction.z);
}

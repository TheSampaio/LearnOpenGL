#include "PCH.h"
#include "LightDirectional.h"

#include "Renderer.h"
#include "Shader.h"

LightDirectional::LightDirectional(Shader& shader, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
{
	m_Shader = &shader;
	m_Direction = direction;
	m_Ambient = ambient;
	m_Diffuse = diffuse;
	m_Specular = specular;
}

void LightDirectional::Use()
{
	m_Shader->Bind();

	Renderer::GetInstance().SetUniform3f(*m_Shader, "uLightDirectional.direction", m_Direction.x, m_Direction.y, m_Direction.z);
	Renderer::GetInstance().SetUniform3f(*m_Shader, "uLightDirectional.base.ambient", m_Ambient.r, m_Ambient.g, m_Ambient.b);
	Renderer::GetInstance().SetUniform3f(*m_Shader, "uLightDirectional.base.diffuse", m_Diffuse.r, m_Diffuse.g, m_Diffuse.b);
	Renderer::GetInstance().SetUniform3f(*m_Shader, "uLightDirectional.base.specular", m_Specular.r, m_Specular.g, m_Specular.b);

	m_Shader->Unbind();
}

#include "PCH.h"
#include "LightSpot.h"

#include "Shader.h"
#include "Renderer.h"

LightSpot::LightSpot(glm::vec3 position, glm::vec3 direction, GLfloat inner, GLfloat outer, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
{
	m_Position = position;
	m_Direction = direction;
	m_Inner = inner;
	m_Outer = outer;
	m_Ambient = ambient;
	m_Diffuse = diffuse;
	m_Specular = specular;
}

void LightSpot::Use(Shader& shader) const
{
	Renderer::GetInstance().SetUniform3f(shader, "uLightSpot.base.ambient", m_Ambient.r, m_Ambient.g, m_Ambient.b);
	Renderer::GetInstance().SetUniform3f(shader, "uLightSpot.base.diffuse", m_Diffuse.r, m_Diffuse.g, m_Diffuse.b);
	Renderer::GetInstance().SetUniform3f(shader, "uLightSpot.base.specular", m_Specular.r, m_Specular.g, m_Specular.b);
	Renderer::GetInstance().SetUniform3f(shader, "uLightSpot.position", m_Position.x, m_Position.y, m_Position.z);
	Renderer::GetInstance().SetUniform3f(shader, "uLightSpot.direction", m_Direction.x, m_Direction.y, m_Direction.z);
	Renderer::GetInstance().SetUniform1f(shader, "uLightSpot.inner", m_Inner);
	Renderer::GetInstance().SetUniform1f(shader, "uLightSpot.outer", m_Outer);
}

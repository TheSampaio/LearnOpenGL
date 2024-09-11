#include "PCH.h"
#include "LightPoint.h"

#include "Shader.h"
#include "Renderer.h"

LightPoint::LightPoint(glm::vec3 position, GLfloat constant, GLfloat linear, GLfloat quadratic, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
{
	m_Position = position;
	m_Constant = constant;
	m_Linear = linear;
	m_Quadratic = quadratic;
	m_Ambient = ambient;
	m_Diffuse = diffuse;
	m_Specular = specular;
}

void LightPoint::Use(Shader& shader, unsigned short id) const
{
	std::string light = "uLightPoint[";
	light += std::to_string(id);
	light += "]";

	Renderer::GetInstance().SetUniform3f(shader, (light + ".base.ambient").c_str(), m_Ambient.r, m_Ambient.g, m_Ambient.b);
	Renderer::GetInstance().SetUniform3f(shader, (light + ".base.diffuse").c_str(), m_Diffuse.r, m_Diffuse.g, m_Diffuse.b);
	Renderer::GetInstance().SetUniform3f(shader, (light + ".base.specular").c_str(), m_Specular.r, m_Specular.g, m_Specular.b);
	Renderer::GetInstance().SetUniform3f(shader, (light + ".position").c_str(), m_Position.x, m_Position.y, m_Position.z);
	Renderer::GetInstance().SetUniform1f(shader, (light + ".constant").c_str(), m_Constant);
	Renderer::GetInstance().SetUniform1f(shader, (light + ".linear").c_str(), m_Linear);
	Renderer::GetInstance().SetUniform1f(shader, (light + ".quadratic").c_str(), m_Quadratic);
}

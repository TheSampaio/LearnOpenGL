#include "PCH.h"
#include "Material.h"

#include "Shader.h"
#include "Texture.h"
#include "Renderer.h"

Material::Material(Texture* diffuse, Texture* specular, GLfloat intensity, GLfloat shininess)
	: m_Diffuse(nullptr), m_Specular(nullptr)
{
	if (diffuse)
		m_Diffuse = diffuse;

	if (specular)
		m_Specular = specular;

	m_Intensity = intensity;
	m_Shininess = shininess;
}

void Material::Bind(Shader& shader)
{
	if (m_Diffuse)
	{
		// Set-ups texture's uniform and binds the texture
		Renderer::GetInstance().SetUniform1i(shader, "uMaterial.diffuse", 0);
		m_Diffuse->Bind();
	}

	if (m_Specular)
	{
		// Set-ups texture's uniform and binds the texture
		Renderer::GetInstance().SetUniform1i(shader, "uMaterial.specular", 1);
		m_Specular->Bind();
	}

	Renderer& renderer = Renderer::GetInstance();
	renderer.SetUniform1f(shader, "uMaterial.intensity", m_Intensity);
	renderer.SetUniform1f(shader, "uMaterial.shininess", m_Shininess);
}

void Material::Unbind()
{
	if (m_Specular)
		m_Specular->Unbind();

	if (m_Diffuse)
		m_Diffuse->Unbind();
}

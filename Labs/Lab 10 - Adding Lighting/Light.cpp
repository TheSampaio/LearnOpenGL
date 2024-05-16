#include "PCH.h"
#include "Light.h"

#include "Renderer.h"

Light::Light()
{
	m_Ambient = glm::vec3{ 1.0f };
	m_Diffuse = glm::vec3{ 1.0f };
	m_Specular = glm::vec3{ 1.0f };
}

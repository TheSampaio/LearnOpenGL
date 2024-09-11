#pragma once

#include "Shader.h"

class Light
{
public:
	Light();

protected:
	glm::vec3 m_Ambient;
	glm::vec3 m_Diffuse;
	glm::vec3 m_Specular;
};

#pragma once

#include "Light.h"

class LightDirectional : Light
{
public:
	LightDirectional(glm::vec3 direction,
		glm::vec3 ambient = glm::vec3{ 1.0f },
		glm::vec3 diffuse = glm::vec3{ 1.0f }, 
		glm::vec3 specular = glm::vec3{ 1.0f });

	void Use(class Shader& shader) const;

private:
	glm::vec3 m_Direction;
};

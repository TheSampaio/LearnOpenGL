#ifndef LEARNOPENGL_LIGHTSPOT_H
#define LEARNOPENGL_LIGHTSPOT_H

#include "Light.h"

class LightSpot : Light
{
public:
	LightSpot(glm::vec3 position,
		glm::vec3 direction = glm::vec3{ 0.0f, 1.0f, 0.0f },
		GLfloat inner = 0.04f,
		GLfloat outer = 0.8f,
		glm::vec3 ambient = glm::vec3{ 1.0f },
		glm::vec3 diffuse = glm::vec3{ 1.0f },
		glm::vec3 specular = glm::vec3{ 1.0f });

	void Use(class Shader& shader) const;

private:
	glm::vec3 m_Position;
	glm::vec3 m_Direction;
	GLfloat m_Inner;
	GLfloat m_Outer;
};

#endif // !LEARNOPENGL_LIGHTSPOT_H

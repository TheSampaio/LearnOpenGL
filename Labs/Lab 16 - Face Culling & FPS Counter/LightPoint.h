#ifndef LEARNOPENGL_LIGHTPOINT_H
#define LEARNOPENGL_LIGHTPOINT_H

#include "Light.h"

class LightPoint : Light
{
public:
	LightPoint(glm::vec3 position, GLfloat constant = 1.0f, GLfloat linear = 0.09f, GLfloat quadratic = 0.032f,
		glm::vec3 ambient = glm::vec3{ 1.0f },
		glm::vec3 diffuse = glm::vec3{ 1.0f },
		glm::vec3 specular = glm::vec3{ 1.0f });

	virtual void Use(class Shader& shader, unsigned short id) const;

private:
	glm::vec3 m_Position;
	GLfloat m_Constant;
	GLfloat m_Linear;
	GLfloat m_Quadratic;
};

#endif // !LEARNOPENGL_LIGHTPOINT_H

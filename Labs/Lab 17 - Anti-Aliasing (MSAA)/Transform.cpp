#include "PCH.h"
#include "Transform.h"

#include "Shader.h"
#include "Renderer.h"

Transform::Transform()
{
	m_Model = glm::mat4{ 1.0f };
	m_NormalMatrix = glm::mat4{ 1.0f };
}

void Transform::Update(Shader& shader)
{
	m_NormalMatrix = glm::transpose(glm::inverse(m_Model));

	// Send data from CPU to GPU by using uniforms
	Renderer::GetInstance().SetUniformMatrix4fv(shader, "uModel", m_Model);
	Renderer::GetInstance().SetUniformMatrix4fv(shader, "uNormalMatrix", m_NormalMatrix);
}

void Transform::Translate(glm::vec3 position)
{
	m_Model = glm::translate(m_Model, position);
}

void Transform::Rotate(glm::vec3 rotation)
{
	m_Model = glm::rotate(m_Model, glm::radians(1.0f * glm::length(rotation)), rotation);
}

void Transform::Scale(glm::vec3 scale)
{
	m_Model = glm::scale(m_Model, scale);
}

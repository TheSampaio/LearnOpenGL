#ifndef LEARNOPENGL_TRANSFORM_H
#define LEARNOPENGL_TRANSFORM_H

class Transform
{
public:
	Transform();

	glm::vec3 GetPosition() { return glm::vec3{ m_Model[3] }; }
	glm::vec3 GetRotationEuler();
	glm::vec3 GetScale();

	void Update(class Shader& shader);

	void Translate(glm::vec3 position);
	void Rotate(glm::vec3 rotation);
	void Scale(glm::vec3 scale);

private:
	glm::mat4 m_Model, m_NormalMatrix;
};

#endif // !LEARNOPENGL_TRANSFORM_H

#ifndef LEARNOPENGL_TRANSFORM_H
#define LEARNOPENGL_TRANSFORM_H

class Transform
{
public:
	Transform();

	void Update(class Shader& shader);

	void Translate(glm::vec3 position);
	void Rotate(glm::vec3 rotation);
	void Scale(glm::vec3 scale);

private:
	glm::mat4 m_Model, m_NormalMatrix;
};

#endif // !LEARNOPENGL_TRANSFORM_H

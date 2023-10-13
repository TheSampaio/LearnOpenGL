#ifndef LEARNOPENGL_CAMERA_H
#define LEARNOPENGL_CAMERA_H

class Camera
{
public:
	Camera(float FieldOfView, float MinClipDistance = 0.05f, float MaxClipDistance = 100.0f);

	// Process all camera's event
	void Inputs();
	void Update(class Shader& ShaderProgram);

	// Returns the camera's view matrix
	inline glm::mat4 GetViewMatrix() { return m_View; }

	// Returns the camera's projection matrix
	inline glm::mat4 GetProjectionMatrix() { return m_Projection; }

private:
	// Attributes
	glm::mat4 m_View;
	glm::mat4 m_Projection;

	glm::vec3 m_Position;
	glm::vec3 m_Rotation;
	const glm::vec3 m_Up = glm::vec3{ 0.0f, 1.0f, 0.0f };

	std::array<float, 2> ClipDistance;

	bool m_bFirstClick;
	float m_Sensitivity;
	float m_Speed;
	float m_FieldOfView;
};

#endif // !LEARNOPENGL_CAMERA_H

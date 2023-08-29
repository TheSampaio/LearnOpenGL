#include "PCH.h"
#include "Camera.h"

#include "Window.h"
#include "Renderer.h"
#include "Timer.h"

Camera::Camera(float FieldOfView, float MinClipDistance, float MaxClipDistance)
	: m_bFirstClick(true)
{
	ClipDistance = { MinClipDistance, MaxClipDistance };
	m_Sensitivity = 200.0f;
	m_Speed = 0.0f;
	m_FieldOfView = FieldOfView;

	m_Position = glm::vec3{ 0.0f, 0.5f,  1.0f };
	m_Rotation = glm::vec3{ 0.0f, 0.0f, -1.0f };

	m_View = glm::mat4(1.0f);
	m_Projection = glm::mat4(1.0f);
}

void Camera::ProccessEvents(Shader& ShaderProgram)
{
	// === Keyboard Inputs ===
	// • Forward and backward
	if (glfwGetKey(Window::GetInstance().GetId(), GLFW_KEY_W) == GLFW_PRESS)
		m_Position += m_Speed * m_Rotation;

	else if (glfwGetKey(Window::GetInstance().GetId(), GLFW_KEY_A) == GLFW_PRESS)
		m_Position += m_Speed * -glm::normalize(glm::cross(m_Rotation, m_Up));

	// • Right and left
	if (glfwGetKey(Window::GetInstance().GetId(), GLFW_KEY_D) == GLFW_PRESS)
		m_Position += m_Speed * glm::normalize(glm::cross(m_Rotation, m_Up));

	else if (glfwGetKey(Window::GetInstance().GetId(), GLFW_KEY_S) == GLFW_PRESS)
		m_Position += m_Speed * -m_Rotation;

	// • Up and down
	if (glfwGetKey(Window::GetInstance().GetId(), GLFW_KEY_E) == GLFW_PRESS)
		m_Position += m_Speed * m_Up;

	else if (glfwGetKey(Window::GetInstance().GetId(), GLFW_KEY_Q) == GLFW_PRESS)
		m_Position += m_Speed * -m_Up;

	// • Max speed
	if (glfwGetKey(Window::GetInstance().GetId(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		m_Speed = 0.2f;

	// • Normal speed
	else if (glfwGetKey(Window::GetInstance().GetId(), GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
		m_Speed = 0.1f;

	// • Min speed
	if (glfwGetKey(Window::GetInstance().GetId(), GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
		m_Speed = 0.01f;

	// === Mouse Inputs ===
	if (glfwGetMouseButton(Window::GetInstance().GetId(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		// Hides the mouse's cursor
		glfwSetInputMode(Window::GetInstance().GetId(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// Prevents camera from jumping on the first click
		if (m_bFirstClick)
		{
			glfwSetCursorPos(Window::GetInstance().GetId(), static_cast<double>(Window::GetInstance().GetSize()[0] / 2), static_cast<double>(Window::GetInstance().GetSize()[1] / 2));
			m_bFirstClick = false;
		}

		// Stores the coordinates of the cursor
		double MouseX = 0.0;
		double MouseY = 0.0;

		// Fetches the coordinates of the cursor
		glfwGetCursorPos(Window::GetInstance().GetId(), &MouseX, &MouseY);

		// Calculates camera's pitch and yaw
		float Pitch = m_Sensitivity * static_cast<float>(MouseY - static_cast<double>(Window::GetInstance().GetSize()[1] / 2)) / Window::GetInstance().GetSize()[1];
		float Yaw =   m_Sensitivity * static_cast<float>(MouseX - static_cast<double>(Window::GetInstance().GetSize()[0] / 2)) / Window::GetInstance().GetSize()[0];

		// Calculates the new camera's orientaion (Bug fix)
		glm::vec3 NewOrientation = glm::rotate(m_Rotation, glm::radians(-Pitch), glm::normalize(glm::cross(m_Rotation, m_Up)));

		if (abs(glm::angle(NewOrientation, m_Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
			m_Rotation = NewOrientation;

		// Rotates the camera left and right
		m_Rotation = glm::rotate(m_Rotation, glm::radians(-Yaw), m_Up);

		// Sets mouse cursor to the middle of the screen
		glfwSetCursorPos(Window::GetInstance().GetId(), static_cast<double>(Window::GetInstance().GetSize()[0] / 2), static_cast<double>(Window::GetInstance().GetSize()[1] / 2));
	}

	// Shows the mouse's cursor
	else if (glfwGetMouseButton(Window::GetInstance().GetId(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		glfwSetInputMode(Window::GetInstance().GetId(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		m_bFirstClick = true;
	}

	// === View Projection Matrix ===
	m_View = glm::lookAt(m_Position, m_Position + m_Rotation, m_Up);
	m_Projection = glm::perspective(glm::radians(m_FieldOfView), static_cast<float>(Window::GetInstance().GetSize()[0]) / static_cast<float>(Window::GetInstance().GetSize()[1]), ClipDistance[0], ClipDistance[1]);

	// Sends the VP matrix to the GPU
	Renderer::GetInstance().SetUniformMatrix4fv(ShaderProgram, "View", m_View);
	Renderer::GetInstance().SetUniformMatrix4fv(ShaderProgram, "Projection", m_Projection);
}

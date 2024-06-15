#include "PCH.h"
#include "Camera.h"

#include "Window.h"
#include "Renderer.h"
#include "Timer.h"

Camera::Camera(glm::vec3 position, float fieldOfView, float minClipDistance, float maxClipDistance)
	: m_bFirstClick(true)
{
	m_ClipDistance = { minClipDistance, maxClipDistance };
	m_Sensitivity = 100.0f;
	m_Speed = 0.0f;
	m_FieldOfView = fieldOfView;

	m_Position = position;
	m_Rotation = glm::vec3{ 0.0f, 0.0f, -1.0f };

	m_View = glm::mat4{ 1.0f };
	m_Projection = glm::mat4{ 1.0f };
}

void Camera::ProcessInputs()
{
	// === Keyboard Inputs ===
	// • Forward and backward
	if (glfwGetKey(Window::GetInstance().GetId(), GLFW_KEY_W) == GLFW_PRESS)
		m_Position += m_Speed * m_Rotation * Timer::GetInstance().GetDeltaTime();

	else if (glfwGetKey(Window::GetInstance().GetId(), GLFW_KEY_A) == GLFW_PRESS)
		m_Position += m_Speed * -glm::normalize(glm::cross(m_Rotation, m_Up)) * Timer::GetInstance().GetDeltaTime();

	// • Right and left
	if (glfwGetKey(Window::GetInstance().GetId(), GLFW_KEY_D) == GLFW_PRESS)
		m_Position += m_Speed * glm::normalize(glm::cross(m_Rotation, m_Up)) * Timer::GetInstance().GetDeltaTime();

	else if (glfwGetKey(Window::GetInstance().GetId(), GLFW_KEY_S) == GLFW_PRESS)
		m_Position += m_Speed * -m_Rotation * Timer::GetInstance().GetDeltaTime();

	// • Up and down
	if (glfwGetKey(Window::GetInstance().GetId(), GLFW_KEY_E) == GLFW_PRESS)
		m_Position += m_Speed * m_Up * Timer::GetInstance().GetDeltaTime();

	else if (glfwGetKey(Window::GetInstance().GetId(), GLFW_KEY_Q) == GLFW_PRESS)
		m_Position += m_Speed * -m_Up * Timer::GetInstance().GetDeltaTime();

	// • Max speed
	if (glfwGetKey(Window::GetInstance().GetId(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		m_Speed = 10.0f;

	// • Normal speed
	else if (glfwGetKey(Window::GetInstance().GetId(), GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
		m_Speed = 2.0f;

	// • Min speed
	if (glfwGetKey(Window::GetInstance().GetId(), GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
		m_Speed = 0.5f;

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
		double mouseX = 0.0;
		double mouseY = 0.0;

		// Fetches the coordinates of the cursor
		glfwGetCursorPos(Window::GetInstance().GetId(), &mouseX, &mouseY);

		// Calculates camera's pitch and yaw
		float pitch = m_Sensitivity * static_cast<float>(mouseY - static_cast<double>(Window::GetInstance().GetSize()[1] / 2)) / Window::GetInstance().GetSize()[1];
		float yaw =   m_Sensitivity * static_cast<float>(mouseX - static_cast<double>(Window::GetInstance().GetSize()[0] / 2)) / Window::GetInstance().GetSize()[0];

		// Calculates the new camera's orientaion (Bug fix)
		glm::vec3 newOrientation = glm::rotate(m_Rotation, glm::radians(-pitch), glm::normalize(glm::cross(m_Rotation, m_Up)));

		if (abs(glm::angle(newOrientation, m_Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
			m_Rotation = newOrientation;

		// Rotates the camera left and right
		m_Rotation = glm::rotate(m_Rotation, glm::radians(-yaw), m_Up);

		// Sets mouse cursor to the middle of the screen
		glfwSetCursorPos(Window::GetInstance().GetId(), static_cast<double>(Window::GetInstance().GetSize()[0] / 2), static_cast<double>(Window::GetInstance().GetSize()[1] / 2));
	}

	// Shows the mouse's cursor
	else if (glfwGetMouseButton(Window::GetInstance().GetId(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		glfwSetInputMode(Window::GetInstance().GetId(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		m_bFirstClick = true;
	}
}

void Camera::Use(Shader& shader)
{
	// Calculates the view projection matrix
	m_View = glm::lookAt(m_Position, m_Position + m_Rotation, m_Up);
	m_Projection = glm::perspective(glm::radians(m_FieldOfView), static_cast<float>(Window::GetInstance().GetSize()[0]) / static_cast<float>(Window::GetInstance().GetSize()[1]), m_ClipDistance[0], m_ClipDistance[1]);

	// Sends the VP matrix to the GPU
	Renderer::GetInstance().SetUniformMatrix4fv(shader, "uView", m_View);
	Renderer::GetInstance().SetUniformMatrix4fv(shader, "uProjection", m_Projection);
	Renderer::GetInstance().SetUniform3f(shader, "uViewPosition", m_Position.x, m_Position.y, m_Position.z);
}

#include "Camera.h"

Camera::Camera(int width, int height, float sensitivity, float speed, glm::vec3 position) // Initialize all camera's variables
{
	Camera::width = width;
	Camera::height = height;
	Camera::sensitivity = sensitivity;
	Camera::speed = speed;
	Camera::position = position;

	aspectRatio = static_cast<float>(width / height);
	initSpeed = speed;
}

void Camera::UpdateMatrix(float FOV, float nearPlane, float farPlane) // Initialize camera's matrix
{
	glm::mat4 view = glm::mat4(1.0f);       // Camera's view's matrix
	glm::mat4 projection = glm::mat4(1.0f); // Camera's projection's matrix

	view = glm::lookAt(position, position + orientation, upVector);									// Makes camera look in the right direction
	projection = glm::perspective(glm::radians(FOV), aspectRatio, nearPlane, farPlane); // Adds perspective to the scene

	cameraMatrix = projection * view;
}

void Camera::Matrix(Shader& shader, const char* uniform)
{
	// Exports the camera matrix to the Vertex Shader
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::Inputs(GLFWwindow* window) // Process all camera's inputs
{
	// --- MOUSE ---------- //

	// Visibility
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) // If press mouse's right button
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); // Hidden mouse's cursor

		if (firstClick) // Avoid camera jump on the first click
		{
			glfwSetCursorPos(window, (width / 2), (height / 2));
			firstClick = false;
			movement = true;
		}

		double mouseX; // Store mouse's coordinates on X axis
		double mouseY; // Store mouse's coordinates on Y axis

		glfwGetCursorPos(window, &mouseX, &mouseY); // Gets the mouse's cursor's coordinates

		// Makes the mouse's cursor begin screen's middle
		float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
		float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

		// Calculates upcoming vertical change in the Orientation
		glm::vec3 newOrientation = glm::rotate(Camera::orientation, glm::radians(-rotX), glm::normalize(glm::cross(Camera::orientation, upVector)));

		// Decides whether or not the next vertical Orientation is legal or not
		if (abs(glm::angle(newOrientation, upVector) - glm::radians(90.0f)) <= glm::radians(85.0f))
		{
			Camera::orientation = newOrientation;
		}

		// Rotates the Orientation left and right
		Camera::orientation = glm::rotate(Camera::orientation, glm::radians(-rotY), upVector);

		// Set mouse's positions in screen's middle
		glfwSetCursorPos(window, (width / 2), (height / 2));
	}

	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) // If release mouse's right button
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // Show mouse's cursor
		firstClick = true;
		movement = false;
	}

	// --- KEYBOARD ---------- //

	if (movement)
	{
		// W, A, S, D
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) // Forward
		{
			Camera::position += speed * Camera::orientation;
		}

		else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) // Backward
		{
			Camera::position += speed * -Camera::orientation;
		}

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) // Rightward
		{
			Camera::position += speed * glm::normalize(glm::cross(Camera::orientation, upVector));
		}

		else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) // Leftward
		{
			Camera::position += speed * -glm::normalize(glm::cross(Camera::orientation, upVector));
		}

		// Speed up
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) // Increse camera's speed on shift press
		{
			Camera::speed = initSpeed * 2;
		}

		else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) // Normalize camera's speed on shift release
		{
			Camera::speed = initSpeed;
		}

		// Up and Down
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) // Go up
		{
			Camera::position += (speed / 2) * upVector;
		}

		else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) // Go down
		{
			Camera::position += (speed / 2) * -upVector;
		}
	}	
}
#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "ShaderClass.h"

class Camera
{
public:
	Camera(int width, int height, float sentibility, float speed, glm::vec3 position); // Camera's contructor

	glm::vec3 position;									  // Camera's position's vector
	glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -1.0f); // Camera's orientation's vector
	glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);     // Camera's upper's vector

	int width;  // Camera's width
	int height; // Camera's height

	bool firstClick = true; // Camera's first click boolean variable
	bool movement = false;  // Camera's movement boolean variable

	float speed;       // Camera's speed
	float initSpeed;   // Camera's initial speed
	float sensitivity; // Camera's sensitivity

	void Inputs(GLFWwindow* window); // Process all camera's inputs and movements
	void Matrix(float FOV, float nearPlane, float farPlane, Shader& shader, const char* uniform); // Initialize camera's matrix
};
#endif
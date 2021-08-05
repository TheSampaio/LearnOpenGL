// Include the libraries
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

// Vertices and colors coordinates
GLfloat vertices[] =
{//           COORDINATES                         //       COLORS            //
	-0.5f,	-0.5f *	float(sqrt(3)) / 3,		0.0f,	 0.8f,	 0.3f,	 0.02f,	// Lower left corner
	 0.5f,	-0.5f *	float(sqrt(3)) / 3,		0.0f,	 0.8f,	 0.3f,   0.02f, // Lower right corner
	 0.0f,	 0.5f *	float(sqrt(3)) * 2 / 3,	0.0f,	 1.0f,	 0.6f,   0.32f, // Upper corner
	-0.25f,	 0.5f * float(sqrt(3)) / 6,		0.0f,	 0.9f,	0.45f,   0.17f, // Inner left
	 0.25f,	 0.5f * float(sqrt(3)) / 6,		0.0f,	 0.9f,  0.45f,	 0.17f,	// Inner right
	 0.0f,	-0.5f *	float(sqrt(3)) / 3,		0.0f,    0.8f,	 0.3f,	 0.02f	// Inner down
};

// Indices for vertices order
GLuint indices[] =
{
	0, 3, 5, // Lower left triangle
	3, 2, 4, // Lower right triangle
	5, 4, 1 // Upper triangle
};

// Process keyboard's inputs
void inputKey(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // Choose a key
		glfwSetWindowShouldClose(window, true); // Choose an action
}

// Main Function
int main()
{
	// Initialize GLFW
	glfwInit();

	// Set the window's dimention
	unsigned int windowWidth = 700;
	unsigned int windowHeight = 600;

	// Set the OpenGL version (3.3) and its mode (Modern / Core or Compatibillity)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Chose the modern OpenGL's functions

	// Create a GLFW window
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "My OpenGL Window", NULL, NULL);

	// If the window won't initialize
	if (window == NULL)
	{
		std::cout << "[ERROR]: Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Make a context
	glfwMakeContextCurrent(window);

	// Load GLAD
	gladLoadGL();

	// Create a viewport (Canvas)
	glViewport(0, 0, windowWidth, windowHeight);

	// Link the vertex and fragment shaders
	Shader shaderProgram("default.vert", "default.frag");

	VAO VAO1;
	VAO1.Bind();

	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));

	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");


	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		//Take care of all keybord events
		inputKey(window);

		// Set background color
		glClearColor(0.05f, 0.15f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Use the created program
		shaderProgram.Activate();
		glUniform1f(uniID, 0.5f);

		VAO1.Bind();
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

		// Swap the last buffer by a new one
		glfwSwapBuffers(window);

		// Take care of all GLFW events
		glfwPollEvents();
	}


	// Delete all after use
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();

	// Destroy window and its process
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
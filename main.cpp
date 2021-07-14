// Include the libraries
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Vertex Shader source code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

//Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";

// Process keyboard's inputs
void inputKey(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // Choose a key
		glfwSetWindowShouldClose(window, true); // Choose an action
}

int main()
{
	// Initialize GLFW
	glfwInit();

	// Set the window's dimention
	unsigned int windowWidth = 800;
	unsigned int windowHeight = 620;

	// Set the OpenGL version (3.3) and its mode (Modern / Core or Compatibillity)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLfloat vertices[] =
	{
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f
	};

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

	// Create a vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader); // Compilling shader

	// Create a fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader); // Compiling shader

	// Create a shader program
	GLuint shaderProgram = glCreateProgram();

	// Attach the created shader to the program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram); // Linking the program and the created shaders

	// Delete shaders after link it
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Create a VAO and a VBO
	GLuint VAO, VBO;

	// Generate VAO and VBO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Bind its
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Enable its
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		//Take care of all keybord events
		inputKey(window);

		// Set background color
		glClearColor(0.05f, 0.15f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Use the created program
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Swap the last buffer by a new one
		glfwSwapBuffers(window);

		// Take care of all GLFW events
		glfwPollEvents();
	}

	// Delete all after use
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	// Destroy window and its process
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
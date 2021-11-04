#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "ShaderClass.h"

GLfloat vertices[] // Array vertices data
{
    //                      COORDINATES             /          COLORS           //
    -0.50f,  -0.5f * float(sqrt(3)) / 3,      0.0f,     0.8f,   0.30f,   0.02f, // Lower left corner
     0.50f,  -0.5f * float(sqrt(3)) / 3,      0.0f,     0.8f,   0.30f,   0.02f, // Lower right corner
     0.00f,   0.5f * float(sqrt(3)) * 2 / 3,  0.0f,     1.0f,   0.60f,   0.32f, // Upper corner
    -0.25f,   0.5f * float(sqrt(3)) / 6,      0.0f,     0.9f,   0.45f,   0.17f, // Inner left
     0.25f,   0.5f * float(sqrt(3)) / 6,      0.0f,     0.9f,   0.45f,   0.17f, // Inner right
     0.00f,  -0.5f * float(sqrt(3)) / 3,      0.0f,     0.8f,   0.30f,   0.02f  // Inner down
};

GLuint indices[] // Array indices data
{
    0, 3, 5, // Lower left triangle
    3, 2, 4, // Lower right triangle
    5, 4, 1  // Upper triangle
};

int main(void)
{
    unsigned int windowWidth = 800;                                         // Window's width
    unsigned int windowHeight = (windowWidth / 2) + (windowWidth / 2) / 2;  // Window's height
    const char* windowName = "My OpenGL Window";                            // Window's title

    glfwInit(); // Initializing GLFW

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                  // Setting OpenGL version to '3'.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);                  // Setting OpenGL version to 3.'3'
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // Choosing OpenGL's CORE mode

    if (!glfwInit()) // Verifying if GLFW was initialized
    {
        std::cout << "[ERROR]: Failed to initialize GLFW" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, windowName, NULL, NULL);  // Creating the window

    if (!window) // Verifying if window was initialized
    {
        std::cout << "[ERROR]: Failed to create a window" << std::endl;
        glfwTerminate(); // Finilizing GLFW
        return -1;
    }

    glfwMakeContextCurrent(window); // Creating a context
    gladLoadGL();                   // Loading GLAD

    glViewport(0, 0, windowWidth, windowHeight); // Creating a viewport

    Shader shaderProgram("default.vert", "default.frag"); // Creating a Shader Program

    VAO VAO1;      // Creating a Vertex Array Buffer to store the VBO and EBO
    VAO1.Bind();   // Binding de VAO

    VBO VBO1(vertices, sizeof(vertices)); // Creating a Vertex Buffer Object and links it to vertices
    EBO EBO1(indices, sizeof(indices));   // Creating a Element Buffer Object and links it to indices

    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*) 0);                    // Setting the VBO's buffer (vertices)
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*) (3 * sizeof(float)));  // Setting the EBO's buffer (colors)

    VAO1.Unbind(); // Binds the VAO to avoid bugs
    VBO1.Unbind(); // Binds the VBO
    EBO1.Unbind(); // Binds the EBO

    GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale"); // Get the uniform variable from "default.vert"

    while (!glfwWindowShouldClose(window)) // Windows's loop
    {
        glClearColor(0.08f, 0.14f, 0.18f, 1.0f); // Choosing window's background's color
        glClear(GL_COLOR_BUFFER_BIT);            // Cleaning up front buffer

        shaderProgram.Active();     // Activate shader program
        glUniform1f(uniID, 0.5f);   // Scaling the triangle in 50%

        VAO1.Bind();                                            // Bind the VAO again
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);    // Tell OpenGL what to draw

        glfwSwapBuffers(window); // Swapping buffers
        glfwPollEvents();        // Processing window's events
    }

    // Deleting what we don't need anymore
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    shaderProgram.Delete();

    glfwDestroyWindow(window);  // Destroying window's process
    glfwTerminate();            // Finilizing GLFW
    return 0;
}
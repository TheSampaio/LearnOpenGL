#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "ShaderClass.h"

GLfloat vertices[] // Array vertices data
{
    -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,    // Lower left corner
     0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,    // Lower right corner
     0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f, // Upper corner
    -0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner left
     0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner right
     0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f     // Inner down
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

    Shader shaderProgram("default.vert", "default.frag");

    VAO VAO1;
    VAO1.Bind();

    VBO VBO1(vertices, sizeof(vertices));
    EBO EBO1(indices, sizeof(indices));

    VAO1.LinkVBO(VBO1, 0);

    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    while (!glfwWindowShouldClose(window)) // Windows's loop
    {
        glClearColor(0.08f, 0.14f, 0.18f, 1.0f); // Choosing window's background's color
        glClear(GL_COLOR_BUFFER_BIT);            // Cleaning up front buffer

        shaderProgram.Active(); // Activate shader program

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
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Texture.h"
#include "ShaderClass.h"

// Defing macros
#define WINDOW_NAME     "My OpenGL Window"
#define WINDOW_WIDTH    800
#define WINDOW_HEIGHT   (windowWidth / 2) + (windowWidth / 2) / 2

// Vertices data array
GLfloat vertices[]
{ //      COORDINATES      /        COLORS         /   TEXTURES    //    
    -0.5f,  -0.5f,  0.0f,     1.0f,  0.0f,  0.0f,     0.0f, 0.0f,  //  Lower left corner   //  0
     0.5f,  -0.5f,  0.0f,     0.0f,  1.0f,  0.0f,     1.0f, 0.0f,  //  Lower right corner  //  1
     0.5f,   0.5f,  0.0f,     0.0f,  0.0f,  1.0f,     1.0f, 1.0f,  //  Upper right corner  //  2
    -0.5f,   0.5f,  0.0f,     1.0f,  1.0f,  1.0f,     0.0f, 1.0f   //  Upper left corner   //  3
};

// Indices data array
GLuint indices[]
{
    0, 2, 1, // Lower left triangle
    0, 3, 2  // Upper right triangle
};

// Referencing the ProcessInput function created at code's end
void ProcessCloseWindowInput(GLFWwindow* window);

int main(void)
{
    unsigned int windowWidth = WINDOW_WIDTH;    // Window's width
    unsigned int windowHeight = WINDOW_HEIGHT;  // Window's height
    const char* windowName = WINDOW_NAME;       // Window's title

    glfwInit(); // Initializing GLFW

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                  // Setting OpenGL's version to '3'.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);                  // Setting OpenGL's version to 3.'3'
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // Choosing OpenGL's CORE mode

    if (!glfwInit()) // Verifying if GLFW was initialized
    {
        std::cout << "[ERROR]: Failed to initialize GLFW" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, windowName, NULL, NULL);  // Creating a window

    if (!window) // Verifying if window was initialized
    {
        std::cout << "[ERROR]: Failed to create a window" << std::endl;
        glfwTerminate(); // Finilizing GLFW
        return -1;
    }

    glfwMakeContextCurrent(window);  // Creating a context
    gladLoadGL();                    // Loading GLAD

    glViewport(0, 0, windowWidth, windowHeight); // Creating a viewport

    Shader shaderProgram("default.vert", "default.frag"); // Creating a Shader Program

    VAO VAO1;     // Creating a Vertex Array Buffer to store the VBO and EBO
    VAO1.Bind();  // Binding de VAO

    VBO VBO1(vertices, sizeof(vertices));  // Creating a Vertex Buffer Object and links it to vertices
    EBO EBO1(indices, sizeof(indices));    // Creating a Element Buffer Object and links it to indices

    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*) 0);                   // Setting the VAO's buffer (Cordinates)
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));  // Setting the VAO's buffer (Colors)
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));  // Setting the VAO's buffer (Textures)

    VAO1.Unbind(); // Binding the VAO to avoid bugs
    VBO1.Unbind(); // Binding the VBO
    EBO1.Unbind(); // Binding the EBO

    GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale"); // Get the uniform variable from "default.vert"

    Texture sandBricks("D_Brick_01.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE); // Create a texture
    sandBricks.texUnit(shaderProgram, "tex0", 0); // Gets the uniform location for created texture

    while (!glfwWindowShouldClose(window)) // Windows's loop
    {
        ProcessCloseWindowInput(window);
        glClearColor(0.08f, 0.14f, 0.18f, 1.0f); // Choosing window's background's color
        glClear(GL_COLOR_BUFFER_BIT);            // Cleaning up front buffer

        shaderProgram.Active();    // Activate shader program
        glUniform1f(uniID, 0.5f);  // Scaling the triangle in 50%
        sandBricks.Bind();         // Binds the created texture

        VAO1.Bind();                                          // Bind the VAO again
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);  // Tell OpenGL what to draw

        glfwSwapBuffers(window); // Swapping buffers
        glfwPollEvents();        // Processing window's events
    }

    // Deleting what we don't need anymore
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    sandBricks.Delete();
    shaderProgram.Delete();

    glfwDestroyWindow(window);  // Destroying window's process
    glfwTerminate();            // Finilizing GLFW
    return 0;
}

// Funtion that process our key inputs
void ProcessCloseWindowInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  // If "Esc" iguals "1"
    {
        glfwSetWindowShouldClose(window, true);             // Closes window
    }
}
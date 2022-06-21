#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Texture.h"
#include "ShaderClass.h"

// Define all macros
#define WINDOW_NAME     "My OpenGL Window"
#define WINDOW_WIDTH    800
#define WINDOW_HEIGHT   (windowWidth / 2) + (windowWidth / 2) / 2

// Vertices data array
GLfloat vertices[]
{ //     COORDINATES     /         COLORS         /   TEXTURES  //    
    -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,
    -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,
     0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,
     0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,
     0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f
};

// Indices data array
GLuint indices[]
{
    0, 1, 2,
    0, 2, 3,
    0, 1, 4,
    1, 2, 4,
    2, 3, 4,
    3, 0, 4
};

// Referencing the ProcessInput function created at code's end
void ProcessCloseWindowInput(GLFWwindow* window);

int main(void)
{
    const unsigned int windowWidth = WINDOW_WIDTH;   // Window's width
    const unsigned int windowHeight = WINDOW_HEIGHT; // Window's height
    const char* windowName = WINDOW_NAME;            // Window's title

    glfwInit(); // Initialize GLFW

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                 // Set OpenGL's version to '3'.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);                 // Set OpenGL's version to 3.'3'
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Choose OpenGL's CORE mode (Modern OpenGL)

    if (!glfwInit()) // Verify if GLFW was initialized
    {
        std::cout << "[ERROR]: Failed to initialize GLFW" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, windowName, NULL, NULL); // Create a window

    if (!window) // Verify if window was created
    {
        std::cout << "[ERROR]: Failed to create a window" << std::endl;
        glfwTerminate(); // Finilize GLFW
        return -1;
    }

    glfwMakeContextCurrent(window); // Creating a context
    gladLoadGL();                   // Loading GLAD

    glViewport(0, 0, windowWidth, windowHeight); // Create a viewport

    Shader shaderProgram("default.vert", "default.frag"); // Create a Shader Program

    VAO VAO1;    // Create a Vertex Array Buffer to store the VBO and EBO
    VAO1.Bind(); // Binds de VAO

    VBO VBO1(vertices, sizeof(vertices)); // Create a Vertex Buffer Object and links it to vertices
    EBO EBO1(indices, sizeof(indices));   // Create a Element Buffer Object and links it to indices

    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*) 0);                  // Configure the VAO's buffer (Cordinates)
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float))); // Configure the VAO's buffer (Colors)
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float))); // Configure the VAO's buffer (Textures)

    VAO1.Unbind(); // Binds the VAO to avoid bugs
    VBO1.Unbind(); // Binds the VBO
    EBO1.Unbind(); // Binds the EBO

    GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale"); // Gets uniform "scale" from "default.vert"

    Texture sandBricks("D_Brick_01.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE); // Create a texture
    sandBricks.texUnit(shaderProgram, "tex0", 0);                                               // Gets uniform "tex0" for the created texture's location

    float rotation = 0.0f;              // Store the model's rotation to use in a simple timer
    double elapsedTime = glfwGetTime(); // Store the elapsed time to program a simple timer

    glEnable(GL_DEPTH_TEST); // To avoid depth visual glitchs

    while (!glfwWindowShouldClose(window)) // Windows's loop
    {
        ProcessCloseWindowInput(window);                    // Process window's shutdown input
        glClearColor(0.08f, 0.14f, 0.18f, 1.0f);            // Set window's background's color
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear front buffer and depth buffer

        shaderProgram.Active(); // Activate shader program

        double currentTime = glfwGetTime(); // Store the application's current time

        if (currentTime - elapsedTime >= 1 / 60) // Simple timer
        {
            rotation += 0.5f;          // Increse rotation's value
            elapsedTime = currentTime; // Update timer
        }

        glm::mat4 model = glm::mat4(1.0f);      // Initialize the model's matrix to avoid NULL matrix
        glm::mat4 view = glm::mat4(1.0f);       // Initialize the view's matrix to avoid NULL matrix
        glm::mat4 projection = glm::mat4(1.0f); // Initialize the projection's matrix to avoid NULL matrix

        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f)); // Apply model's rotation

        view = glm::translate(view, glm::vec3(0.0f, -0.5f, -2.0f));                                            // Set view's position
        projection = glm::perspective(glm::radians(45.0f), (float)(windowWidth / windowHeight), 0.1f, 100.0f); // Set view's angle and clip's range

        int modelLocation = glGetUniformLocation(shaderProgram.ID, "model");   // Gets uniform "model" from "default.vert"
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model)); // Configure model's uniform

        int viewLocation = glGetUniformLocation(shaderProgram.ID, "view");   // Gets uniform "view" from "default.vert"
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view)); // Configure view's uniform

        int projectionLocation = glGetUniformLocation(shaderProgram.ID, "projection");   // Gets uniform "projection" from "default.vert"
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection)); // Configure projection's uniform

        glUniform1f(uniID, 0.5f); // Scale the triangle in 50%
        sandBricks.Bind();        // Binds the created texture

        VAO1.Bind(); // Binds the VAO again

        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0); // Tell OpenGL what to draw

        glfwSwapBuffers(window); // Swapp buffers
        glfwPollEvents();        // Process window's events
    }

    // Delete what we don't need anymore
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    sandBricks.Delete();
    shaderProgram.Delete();

    glfwDestroyWindow(window); // Destroy window's process
    glfwTerminate();           // Finilize GLFW
    return 0;
}

// Funtion that process our key inputs
void ProcessCloseWindowInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // If "Esc" iguals "1"
    {
        glfwSetWindowShouldClose(window, true); // Closes window
    }
}
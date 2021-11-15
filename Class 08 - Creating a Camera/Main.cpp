#include <iostream>
#include <string>
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
#include "Camera.h"
#include "Timer.h"

// --- Define all macros ---------- //

// Window's macros
#define WINDOW_NAME                "My OpenGL Window"                      // Window's title
#define WINDOW_WIDTH               800                                     // Window's width
#define WINDOW_HEIGHT              (WINDOW_WIDTH/2) + ((WINDOW_WIDTH/2)/2) // Window's height
#define WINDOW_BACKGROUND_COLOR    0.08f, 0.14f, 0.18f, 1.0f               // Window's background color (0.08f, 0.14f, 0.18f, 1.0f)
                                   
// Camera's macros                 
#define CAMERA_SPEED               0.02f  // Camera's speed
#define CAMERA_SENSITIVITY         100.0f // Camera's sensitivity
#define CAMERA_FOV                 80.0f  // Camera's field of view
#define CAMERA_MIN_DISTANCE        0.01f  // Camera's view minimum distance
#define CAMERA_MAX_DISTANCE        100.0f // Camera's view maximum distance

// -------------------------------- //

// Vertices data array
GLfloat vertices[]
{ //     COORDINATES      /       COLORS        /   TEXTURES   //    
    -0.5f,  0.0f,  0.5f,     0.8f, 0.8f, 0.8f,     0.0f, 0.0f, // 0
     0.5f,  0.0f,  0.5f,     0.8f, 0.8f, 0.8f,     5.0f, 0.0f, // 1
     0.0f,  0.8f,  0.0f,     1.0f, 1.0f, 1.0f,     2.5f, 2.5f, // 2
     0.5f,  0.0f, -0.5f,     0.8f, 0.8f, 0.8f,     0.0f, 0.0f, // 3
    -0.5f,  0.0f, -0.5f,     0.8f, 0.8f, 0.8f,     5.0f, 0.0f  // 4
};

// Indices data array
GLuint indices[]
{
    0, 1, 2, // Side's faces
    1, 3, 2,
    3, 4, 2,
    0, 4, 2,

    0, 1, 4, // Base's faces
    1, 3, 4
};

// Referencing the ProcessInput and FramebufferSizeCallback functions that was created at code's end
void ProcessCloseWindowInput(GLFWwindow* window);
void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

int main(void)
{
    glfwInit(); // Initialize GLFW

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                 // Set OpenGL's version to '3'.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);                 // Set OpenGL's version to 3.'3'
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Choose OpenGL's CORE mode (Modern OpenGL)

    if (!glfwInit()) // Verify if GLFW was initialized
    {
        std::cout << "[ERROR]: Failed to initialize GLFW" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, NULL, NULL); // Create a window

    if (!window) // Verify if window was created
    {
        std::cout << "[ERROR]: Failed to create window" << std::endl;
        glfwTerminate(); // Finilize GLFW
        return -1;
    }

    glfwMakeContextCurrent(window);                                  // Creating a context
    gladLoadGL();                                                    // Loading GLAD
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback); // Create a viewport

    std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl; // Gets OpenGL's version

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

    Texture sandBricks("D_Brick_01.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE); // Create a texture
    sandBricks.texUnit(shaderProgram, "tex0", 0);                                               // Gets uniform "tex0" for the created texture's location

    glEnable(GL_DEPTH_TEST); // To avoid depth visual glitchs

    Timer timer (0.5f); // Create a timer
    timer.Start();      // Starts timer

    Camera camera(WINDOW_WIDTH, WINDOW_HEIGHT, CAMERA_SENSITIVITY, CAMERA_SPEED, glm::vec3(0.0f, 0.4f, 1.5f));

    float rotation = 0.0f; // Store the model's rotation to use in a simple timer

    while (!glfwWindowShouldClose(window)) // Windows's loop
    {
        timer.Update(); // Updates timer

        camera.Inputs(window);           // Gets all camera's inputs
        ProcessCloseWindowInput(window); // Process window's shutdown input

        glClearColor(WINDOW_BACKGROUND_COLOR);              // Set window's background's color
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear front buffer and depth buffer

        shaderProgram.Active(); // Activate shader program

        camera.Matrix(CAMERA_FOV, CAMERA_MIN_DISTANCE, CAMERA_MAX_DISTANCE, shaderProgram, "camMatrix"); // Initialize camera's matrix and shaders

        rotation = timer.GetTime();         // Inceasing model's rotation
        glm::mat4 model = glm::mat4(1.0f);  // Initialize model's matrix to avoid NULL matrix

        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f)); // Apply model's rotation

        int modelLocation = glGetUniformLocation(shaderProgram.ID, "model");   // Gets uniform "model" from "default.vert"
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model)); // Configure model's uniform

        sandBricks.Bind(); // Binds the created texture
        VAO1.Bind();       // Binds the VAO again

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

void ProcessCloseWindowInput(GLFWwindow* window) // Funtion that process our key inputs
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // If "Esc" iguals "1"
    {
        glfwSetWindowShouldClose(window, true); // Closes window
    }
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height) // Funtion that update our window's viewport
{
    glViewport(0, 0, width, height); // Creates a viewport to the created window
}
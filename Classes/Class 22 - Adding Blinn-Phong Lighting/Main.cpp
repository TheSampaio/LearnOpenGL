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

// --- Define all macros ---------- //

// Window's macros
#define WINDOW_NAME                 "My OpenGL Window"                      // Window's title
#define WINDOW_WIDTH                1024                                     // Window's width
#define WINDOW_HEIGHT               (WINDOW_WIDTH/2) + ((WINDOW_WIDTH/2)/2) // Window's height
#define WINDOW_BACKGROUND_COLOR     0.08f, 0.14f, 0.18f, 1.0f               // Window's background color (0.08f, 0.14f, 0.18f, 1.0f)
                                   
// Camera's macros
#define CAMERA_SPEED                0.02f  // Camera's speed
#define CAMERA_SENSITIVITY          100.0f // Camera's sensitivity
#define CAMERA_FOV                  80.0f  // Camera's field of view
#define CAMERA_MIN_DISTANCE         0.01f  // Camera's view minimum distance
#define CAMERA_MAX_DISTANCE         100.0f // Camera's view maximum distance

// Light's macros
#define LIGHT_COLOR                 1.0f,  1.0f,  1.0f,  1.0f // Light's color (1.0f, 1.0f, 1.0f, 1.0f)
#define LIGHT_POSITION              0.0f,  0.2f,  0.0f        // Light's position (0.5f, 0.5f, 0.5f)

// Pyramid's macros
#define PYRAMID_POSITION            0.0f, 0.0f, 0.0f // Pyramid's position (0.0f, 0.0f, 0.0f)

// -------------------------------- //

// Vertices data array (Plane)
GLfloat vertices[]
{ //     COORDINATES     /        COLORS          /    TexCoord   /        NORMALS       //
    -0.5f,  0.1f,  0.5f,    0.0f,  0.0f,  0.0f,     0.0f,  0.0f,     0.0f,  1.0f,  0.0f, // 0
     0.5f,  0.1f,  0.5f,    0.0f,  0.0f,  0.0f,     2.0f,  0.0f,     0.0f,  1.0f,  0.0f, // 1
    -0.5f,  0.1f, -0.5f,    0.0f,  0.0f,  0.0f,     0.0f,  2.0f,     0.0f,  1.0f,  0.0f, // 2
     0.5f,  0.1f, -0.5f,    0.0f,  0.0f,  0.0f,     2.0f,  2.0f,     0.0f,  1.0f,  0.0f  // 3

};

// Indices data array (Plane)
GLuint indices[]
{
    0, 1, 2,
    2, 3, 1
};

// Vertices data array (Light)
GLfloat lightVertices[]
{ //       COORDINATES       //
    -0.05f, -0.05f, -0.05f,  // 0
     0.05f, -0.05f, -0.05f,  // 1
    -0.05f, -0.05f,  0.05f,  // 2
     0.05f, -0.05f,  0.05f,  // 3

    -0.05f,  0.05f, -0.05f,  // 4
     0.05f,  0.05f, -0.05f,  // 5
    -0.05f,  0.05f,  0.05f,  // 6
     0.05f,  0.05f,  0.05f,  // 7
};

// Indices data array (Light)
GLuint lightIndices[]
{
    0, 1, 2,
    2, 1, 3,

    0, 1, 4,
    4, 5, 1,

    0, 2, 4,
    6, 2, 4,

    4, 5, 6,
    5, 6, 7,

    1, 3, 5,
    5, 7, 3,

    2, 3, 6,
    6, 7, 3
};

Camera camera(WINDOW_WIDTH, WINDOW_HEIGHT, CAMERA_SENSITIVITY, CAMERA_SPEED, glm::vec3(0.0f, 0.4f, 1.5f)); // Create a camera

// Referencing the ProcessInput and FramebufferSizeCallback functions that was created at code's end
void ProcessCloseWindowInput(GLFWwindow* window);
void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

int main(void)
{
    glfwInit(); // Initialize GLFW

    glfwWindowHint(GLFW_SAMPLES, 4);
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

    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);                   // Configure VAO's buffer (Cordinates)
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float))); // Configure VAO's buffer (Colors)
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float))); // Configure VAO's buffer (Textures)
    VAO1.LinkAttrib(VBO1, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float))); // Configure VAO's buffer (Normals)

    VAO1.Unbind(); // Unbinds the VAO
    VBO1.Unbind(); // Unbinds the VBO
    EBO1.Unbind(); // Unbinds the EBO

    Shader lightShader("light.vert", "light.frag"); // Linking light's vertex and fragment shaders

    VAO lightVAO;    // Create a Vertex Array Buffer to store the light's VBO and light's EBO
    lightVAO.Bind(); // Binds de light's VAO

    VBO lightVBO(lightVertices, sizeof(lightVertices)); // Create a Vertex Buffer Object and links it to lightVertices
    EBO lightEBO(lightIndices, sizeof(lightIndices));   // Create a Element Buffer Object and links it to lightIndices

    lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0); // Configure the light's VAO's buffer (Cordinates)

    lightVAO.Unbind(); // Unbinds the light's VAO
    lightVBO.Unbind(); // Unbinds the light's VBO
    lightEBO.Unbind(); // Unbinds the light's EBO

    glm::vec4 lightColor = glm::vec4(LIGHT_COLOR); // Set light's color

    glm::vec3 lightPos = glm::vec3(LIGHT_POSITION);    // Set light's position
    glm::mat4 lightModel = glm::mat4(1.0f);            // Initialize light's matrix
    lightModel = glm::translate(lightModel, lightPos); // Assigns light's matrix's transformations

    glm::vec3 pyramidPos = glm::vec3(PYRAMID_POSITION);      // Set pyramid's position
    glm::mat4 pyramidModel = glm::mat4(1.0f);                // Initialize pyramid's matrix
    pyramidModel = glm::translate(pyramidModel, pyramidPos); // Assigns pyramid's matrix's transformations

    lightShader.Active(); // Actives light's shader program

    // Export important light's values to the GPU using uniforms
    glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "lightModel"), 1, GL_FALSE, glm::value_ptr(lightModel));
    glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);

    shaderProgram.Active(); // Actives pyramid's shader program

    // Export important pyramid's values to the GPU using uniforms
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "pyramidModel"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
    glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

    Texture dWoodFloor("d-planks.png", GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE); // Create a diffuse texture
    dWoodFloor.texUnit(shaderProgram, "tex0", 0);                                     // Gets uniform "tex0" for the created diffuse texture's location

    Texture sWoodFloor("s-planks.png", GL_TEXTURE_2D, 1, GL_RED, GL_UNSIGNED_BYTE); // Create a specular texture
    sWoodFloor.texUnit(shaderProgram, "tex1", 1);                                    // Gets uniform "tex1" for the created specular texture's location

    glEnable(GL_DEPTH_TEST); // To avoid depth visual glitchs

    while (!glfwWindowShouldClose(window)) // Windows's loop
    {
        camera.Inputs(window);           // Gets all camera's inputs
        ProcessCloseWindowInput(window); // Process window's shutdown input

        glClearColor(WINDOW_BACKGROUND_COLOR);              // Set window's background's color
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear front buffer and depth buffer

        camera.UpdateMatrix(CAMERA_FOV, CAMERA_MIN_DISTANCE, CAMERA_MAX_DISTANCE); // Initialize camera's shaders

        shaderProgram.Active();                    // Activate shader program (Plane)
        camera.Matrix(shaderProgram, "camMatrix"); // Initialize camera's matrix

        // Gets camera's position from "default.vert" using uniforms
        glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.position.x, camera.position.y, camera.position.z);

        dWoodFloor.Bind(); // Binds the created diffuse texture
        sWoodFloor.Bind(); // Binds the created specular texture
        VAO1.Bind();       // Binds the VAO again

        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0); // Tell OpenGL what to draw (Plane)

        lightShader.Active();                    // Activate shader program (Light)
        camera.Matrix(lightShader, "camMatrix"); // Initialize camera's matrix
        lightVAO.Bind();                         // Binds the Light's VAO

        glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(int), GL_UNSIGNED_INT, 0); // Tell OpenGL what to draw (Light)

        glfwSwapBuffers(window); // Swapp buffers
        glfwPollEvents();        // Process window's events
    }

// --- Delete what we don't need anymore ---------- //

    // Plane's model
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    dWoodFloor.Delete();
    shaderProgram.Delete();

    // Light's model
    lightVAO.Delete();
    lightVBO.Delete();
    lightEBO.Delete();
    lightShader.Delete();

// ------------------------------------------------ //

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
    camera.width = width;
    camera.height = height;
    camera.aspectRatio = (static_cast<float>(width) / static_cast<float>(height));
    glViewport(0, 0, width, height); // Creates a viewport to the created window
}
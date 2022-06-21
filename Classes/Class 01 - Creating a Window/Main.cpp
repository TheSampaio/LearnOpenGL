#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

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

    glClearColor(0.06f, 0.10f, 0.18f, 1.0f);    // Choosing window's background's color
    glClear(GL_COLOR_BUFFER_BIT);               // Cleaning up front buffer
    glfwSwapBuffers(window);                    // Swapping buffers

    while (!glfwWindowShouldClose(window)) // Windows's loop
    {
        glfwPollEvents(); // Processing window's events
    }

    glfwDestroyWindow(window);  // Destroying window's process
    glfwTerminate();            // Finilizing GLFW
    return 0;
}
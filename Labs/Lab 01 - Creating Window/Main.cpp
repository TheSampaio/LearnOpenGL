// OpenGL's loader must be included before GLFW
#include <glad/glad.h>
#include <glfw/glfw3.h>

#include "Debug.h"

int main()
{
    unsigned short Width = 800, Height = 680;
    const char* Title = "Window | OpenGL";

    // Initializes GLFW and log it if failed
    if (!glfwInit())
    {
        Debug::Log("[ERRO] Failed to initialize GLFW.", true);
        return EXIT_FAILURE;
    }

    // Setup OpenGL's version and compatibility mode
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Creates a window (Windowed mode)
    GLFWwindow* pWindow = glfwCreateWindow(Width, Height, Title, nullptr, nullptr);

    // Verifies if the window was created
    if (!pWindow)
    {
        Debug::Log("[ERRO] Failed to create a window");
        return EXIT_FAILURE;
    }

    // Creates an OpenGL's context
    glfwMakeContextCurrent(pWindow);
    
    // Loads GLAD
    gladLoadGL();

    // Creates a viewport for the window
    glViewport(0, 0, Width, Height);

    // Main loop (Game loop)
    while (!glfwWindowShouldClose(pWindow))
    {
        // Process all window's events
        glfwPollEvents();

        // Clears colour buffer and changes the window's background colour
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.5f, 1.0f);

        // Swaps window's buffers
        glfwSwapBuffers(pWindow);
    }

    // Destroying window's process
    glfwDestroyWindow(pWindow);
    
    // Finalizes GLFW
    glfwTerminate();
    return 0;
}
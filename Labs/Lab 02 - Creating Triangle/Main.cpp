// Standard includes
#include <array>

// OpenGL's loader must be included before GLFW
#include <glad/glad.h>
#include <glfw/glfw3.h>

// Project includes
#include "Debug.h"

int main()
{
    // Store window's title and size
    unsigned short width = 800, height = 600;
    const char* title = "Window | OpenGL";

    // Initializes GLFW and log it if failed
    if (!glfwInit())
    {
        Debug::Log("[ERRO] Failed to initialize GLFW.", true);
        return EXIT_FAILURE;
    }

    // Set-up OpenGL's version and compatibility mode
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Locks window's size
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    // Creates a window (Windowed mode)
    GLFWwindow* pWindow = glfwCreateWindow(width, height, title, nullptr, nullptr);

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
    glViewport(0, 0, width, height);

    // ===== Data Input ================================================================================================== //
    
    // Vertices's array
    const std::array<GLfloat, 9> vertices
    {
        -0.8f, -0.8f,  0.0f,
         0.8f, -0.8f,  0.0f,
         0.0f,  0.8f,  0.0f,
    };

    // Vertex shader's source code (Temporary)
    const char* vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 inPosition;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(inPosition, 1.0);\n"
        "}\0";

    // Fragment shader's source code (Temporary)
    const char* fragmentShaderSource = "#version 330 core\n"
        "out vec4 gl_Color;\n"
        "void main()\n"
        "{\n"
        "gl_Color = vec4(1.0, 0.5, 0.0, 1.0);\n"
        "}\0";

    // ===== Shaders and Shader Program ================================================================================== //

    // Creates identifies for the vertex shader, fragment shader and shader program
    GLuint vertexShader = 0, fragmentShader = 0, shaderProgram = 0;

    // Compiles vertex shader
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    // Compiles fragment shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    // Attaches both shaders to the shader program and links it
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Detaches both shaders to the shader program
    glDetachShader(shaderProgram, vertexShader);
    glDetachShader(shaderProgram, fragmentShader);

    // ===== Buffers Creation ============================================================================================ //
    
    // Creates identifies for the VAO (Vertex Array Object) and VBO (Vertex Buffer Object)
    GLuint VAO = 0, VBO = 0;

    // Generates a VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Generates a VBO and setups it
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    // Set-ups the VAO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(0);

    // Unbind VAO and VBO to avoid bugs
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // =================================================================================================================== //

    // Main loop (Game loop)
    while (!glfwWindowShouldClose(pWindow))
    {
        // Process all window's events
        glfwPollEvents();

        // Clears colour buffer and changes the window's background colour
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.2f, 0.2f, 0.4f, 1.0f);

        // Draw call scope (This scope is JUST to organize!)
        {
            // Informs OpenGL which shader program and VAO we want to use
            glUseProgram(shaderProgram);
            glBindVertexArray(VAO);

            // Draw a triangle using the VBO set-up
            glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size()));

            // Unbind everything binded to avoid bugs
            glBindVertexArray(0);
            glUseProgram(0);
        }

        // Swaps window's buffers
        glfwSwapBuffers(pWindow);
    }

    // Deletes what we need anymore
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteProgram(shaderProgram);

    // Destroying window's process
    glfwDestroyWindow(pWindow);
    
    // Finalizes GLFW
    glfwTerminate();
    return 0;
}

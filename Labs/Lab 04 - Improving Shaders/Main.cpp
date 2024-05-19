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
    
    // Vertices's dynamic array
    const std::array<GLfloat, 24> vertices
    {
        // === Position        // === Color
        -0.8f, -0.8f,  0.0f,    1.0f,  0.0f,  0.0f,
         0.8f, -0.8f,  0.0f,    1.0f,  0.0f,  0.0f,
        -0.8f,  0.8f,  0.0f,    1.0f,  0.8f,  0.0f,
         0.8f,  0.8f,  0.0f,    1.0f,  0.8f,  0.0f
    };

    // Indices's array
    const std::array<GLuint, 6> indices
    {
        0, 1, 2,
        2, 1, 3
    };

    // Vertex shader's source code (Temporary)
    const char* vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 inPosition;\n"
        "layout (location = 1) in vec3 inColor;\n"
        "out vec4 outColor;\n"
        "void main()\n"
        "{\n"
        "   outColor = vec4(inColor, 1.0);\n"
        "   gl_Position = vec4(inPosition, 1.0);\n"
        "}\0";

    // Fragment shader's source code (Temporary)
    const char* fragmentShaderSource = "#version 330 core\n"
        "in vec4 outColor;\n"
        "out vec4 gl_Color;\n"
        "void main()\n"
        "{\n"
        "gl_Color = outColor;\n"
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
    GLuint VAO = 0, VBO = 0, EBO = 0;

    // Generates a VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Generates a VBO and set-ups it
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    // Generates a EBO and set-ups it
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    // Set-ups the VAO's layouts
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), nullptr);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<void*>(3 * sizeof(GLfloat)));

    // Enables the VAO's layouts
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // Unbind VAO and VBO to avoid bugs
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

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

            // Draw a triangle using the EBO set-up
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, nullptr);

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
    glDeleteBuffers(1, &EBO);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteProgram(shaderProgram);

    // Destroying window's process
    glfwDestroyWindow(pWindow);
    
    // Finalizes GLFW
    glfwTerminate();
    return 0;
}

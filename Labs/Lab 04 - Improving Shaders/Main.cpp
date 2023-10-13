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
    unsigned short Width = 800, Height = 600;
    const char* Title = "Window | OpenGL";

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

    // ===== Data Input ================================================================================================== //
    
    // Vertices's array
    const std::array<GLfloat, 24> Vertices
    {
        // === Position        // === Color
        -0.8f, -0.8f,  0.0f,    1.0f,  0.0f,  0.0f,
         0.8f, -0.8f,  0.0f,    1.0f,  0.0f,  0.0f,
        -0.8f,  0.8f,  0.0f,    1.0f,  0.8f,  0.0f,
         0.8f,  0.8f,  0.0f,    1.0f,  0.8f,  0.0f
    };

    // Indices's array
    const std::array<GLuint, 6> Indices
    {
        0, 1, 2,
        2, 1, 3
    };

    // Vertex shader's source code (Temporary)
    const char* VertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 inPosition;\n"
        "layout (location = 1) in vec3 inColor;\n"
        "out vec4 outColor;\n"
        "void main()\n"
        "{\n"
        "   outColor = vec4(inColor, 1.0);\n"
        "   gl_Position = vec4(inPosition, 1.0);\n"
        "}\0";

    // Fragment shader's source code (Temporary)
    const char* FragmentShaderSource = "#version 330 core\n"
        "in vec4 outColor;\n"
        "out vec4 gl_Color;\n"
        "void main()\n"
        "{\n"
        "gl_Color = outColor;\n"
        "}\0";

    // ===== Shaders and Shader Program ================================================================================== //

    // Creates identifies for the vertex shader, fragment shader and shader program
    GLuint VertexShader = 0, FragmentShader = 0, ShaderProgram = 0;

    // Compiles vertex shader
    VertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(VertexShader, 1, &VertexShaderSource, nullptr);
    glCompileShader(VertexShader);

    // Compiles fragment shader
    FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FragmentShader, 1, &FragmentShaderSource, nullptr);
    glCompileShader(FragmentShader);

    // Attaches both shaders to the shader program and links it
    ShaderProgram = glCreateProgram();
    glAttachShader(ShaderProgram, VertexShader);
    glAttachShader(ShaderProgram, FragmentShader);
    glLinkProgram(ShaderProgram);

    // Detaches both shaders to the shader program
    glDetachShader(ShaderProgram, VertexShader);
    glDetachShader(ShaderProgram, FragmentShader);

    // ===== Buffers Creation ============================================================================================ //
    
    // Creates identifies for the VAO (Vertex Array Object) and VBO (Vertex Buffer Object)
    GLuint VAO = 0, VBO = 0, EBO = 0;

    // Generates a VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Generates a VBO and set-ups it
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(GLfloat), Vertices.data(), GL_STATIC_DRAW);

    // Generates a EBO and set-ups it
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(GLuint), Indices.data(), GL_STATIC_DRAW);

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

        // Draw call scope (This scope is to organize!)
        {
            // Informs OpenGL which shader program and VAO we want to use
            glUseProgram(ShaderProgram);
            glBindVertexArray(VAO);

            // Draw a triangle using the EBO set-up
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(Indices.size()), GL_UNSIGNED_INT, nullptr);

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
    glDeleteShader(VertexShader);
    glDeleteShader(FragmentShader);
    glDeleteProgram(ShaderProgram);

    // Destroying window's process
    glfwDestroyWindow(pWindow);
    
    // Finalizes GLFW
    glfwTerminate();
    return 0;
}

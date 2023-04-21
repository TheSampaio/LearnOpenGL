#include <array>

// OpenGL's loader must be included before GLFW
#include <glad/glad.h>
#include <glfw/glfw3.h>

#include "Debug.h"

// Generates a new viewport
void FramebufferCallback(GLFWwindow* Window, int Width, int Height);

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

    // Registers window's callbacks
    glfwSetFramebufferSizeCallback(pWindow, FramebufferCallback);

    // ===== Data Input ================================================================================================== //
    
    // Vertices array
    std::array<GLfloat, 9> Vertices
    {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f,
    };

    // Vertex shader's source code (Temporary)
    const char* VertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";

    // Fragment shader's source code (Temporary)
    const char* FragmentShaderSource = "#version 330 core\n"
        "out vec4 gl_Color;\n"
        "void main()\n"
        "{\n"
        "gl_Color = vec4(1.0f, 0.5f, 0.0f, 1.0f);\n"
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
    GLuint VAO = 0, VBO = 0;

    // Generates a VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Generates a VBO and setups it
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(GLfloat), Vertices.data(), GL_STATIC_DRAW);

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
        glClearColor(0.2f, 0.3f, 0.5f, 1.0f);

        // === Draw bellow ========== //

        // Informs OpenGL which shader program and VAO we want to use
        glUseProgram(ShaderProgram);
        glBindVertexArray(VAO);

        // Draw a triangle using the VBO set-up
        glDrawArrays(GL_TRIANGLES, 0, Vertices.size());

        // ========================== //

        // Swaps window's buffers
        glfwSwapBuffers(pWindow);
    }

    // Deletes what we need anymore
    glDeleteShader(VertexShader);
    glDeleteShader(VertexShader);
    glDeleteProgram(ShaderProgram);

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);

    // Destroying window's process
    glfwDestroyWindow(pWindow);
    
    // Finalizes GLFW
    glfwTerminate();
    return 0;
}

void FramebufferCallback(GLFWwindow* Window, int Width, int Height)
{
    glViewport(0, 0, Width, Height);
}

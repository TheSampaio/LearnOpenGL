// Pre-compiled headers
#include "PCH.h"

// Project includes
#include "Window.h"

int main()
{
    // Gets the window's instance reference
    Window& MainWindow = Window::GetInstance();

    // Set-ups the window
    MainWindow.SetSize(1280, 720);
    MainWindow.SetTitle("Window | OpenGL");
    MainWindow.SetVerticalSynchronization(false);
    MainWindow.SetBackgroundColour(0.2f, 0.3f, 0.5f);

    // Creates the window
    MainWindow.Create();

    // ===== Input Data ================================================================================================== //
    
    // Vertices's array
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
    while (!MainWindow.Close())
    {
        // Process all window's events
        MainWindow.ProcessEvents();

        // Clears colour buffer and changes the window's background colour
        MainWindow.ClearBuffers();

        // ===== Draw bellow ============================================================================================= //

        // Informs OpenGL which shader program and VAO we want to use
        glUseProgram(ShaderProgram);
        glBindVertexArray(VAO);

        // Draw a triangle using the VBO set-up
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(Vertices.size()));

        // =============================================================================================================== //

        // Swaps window's buffers
        MainWindow.SwapBuffers();
    }

    // Deletes what we need anymore
    glDeleteShader(VertexShader);
    glDeleteShader(VertexShader);
    glDeleteProgram(ShaderProgram);

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);

    return 0;
}

// Pre-compiled headers
#include "PCH.h"

// Project includes
#include "Window.h"
#include "Shader.h"

int main()
{
    // Gets the window's instance reference
    Window& MainWindow = Window::GetInstance();

    // Set-ups the window
    MainWindow.SetSize(800, 680);
    MainWindow.SetTitle("Window | OpenGL");
    MainWindow.SetVerticalSynchronization(false);
    MainWindow.SetBackgroundColour(0.2f, 0.2f, 0.4f);

    // Creates the window
    MainWindow.Create();

    // Vertices's array
    std::array<GLfloat, 24> Vertices
    {
        // === Positions       // === Colours
        -0.8f, -0.8f,  0.0f,    1.0f,  0.0f,  0.0f,  // 0
         0.8f, -0.8f,  0.0f,    1.0f,  0.0f,  0.0f,  // 1
        -0.8f,  0.8f,  0.0f,    1.0f,  0.8f,  0.0f,  // 2
         0.8f,  0.8f,  0.0f,    1.0f,  0.8f,  0.0f,  // 3
    };

    // Indices's array
    std::array<GLuint, 6> Indices
    {
        0, 1, 2,
        2, 1, 3
    };

    // Creates a shader program using files for the vertex and fragment shaders
    Shader* DefaultProgram = new Shader("DefaultVert.glsl", "DefaultFrag.glsl");

    // ===== Buffers Creation ============================================================================================ //
    
    // Creates identifies for the VAO (Vertex Array Object), VBO (Vertex Buffer Object) and EBO (Element Buffer Object)
    GLuint VAO = 0, VBO = 0, EBO = 0;

    // Generates a VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Generates a VBO and set-ups it
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(GLfloat), Vertices.data(), GL_STATIC_DRAW);

    // Generates an EBO and set-ups it
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(GLuint), Indices.data(), GL_STATIC_DRAW);

    // Set-ups the VAO's layout
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), nullptr);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

    // Enables the VAO's layout
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // Unbind VAO, VBO and EBO to avoid bugs
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

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
        DefaultProgram->Activate();
        glBindVertexArray(VAO);

        // Draw a triangle using the EBO set-up
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(Indices.size()), GL_UNSIGNED_INT, nullptr);

        // =============================================================================================================== //

        // Swaps window's buffers
        MainWindow.SwapBuffers();
    }

    // Deletes what we need anymore
    delete DefaultProgram;

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &EBO);

    return 0;
}

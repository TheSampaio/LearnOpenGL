// Pre-compiled headers
#include "PCH.h"

// Project includes
#include "Window.h"
#include "Shader.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

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

    // Vertices's dynamic array
    std::vector<GLfloat> Vertices
    {
        -0.8f, -0.8f,  0.0f, // 0
         0.8f, -0.8f,  0.0f, // 1
        -0.8f,  0.8f,  0.0f, // 2
         0.8f,  0.8f,  0.0f, // 3
    };

    // Indices's dynamic array
    std::vector<GLuint> Indices
    {
        0, 1, 2,
        2, 1, 3
    };

    // Creates a shader program using files for the vertex and fragment shaders
    Shader* DefaultProgram = new Shader("DefaultVert.glsl", "DefaultFrag.glsl");

    // Creates all buffer objects
    VAO* VertexArray = new VAO();
    VBO* VertexBuffer = new VBO(Vertices);
    EBO* ElementBuffer = new EBO(Indices);    

    // Set-ups VAO's layouts
    VertexArray->AttribPointer(0, 3, 3 * sizeof(GLfloat), 0);

    // Unbinds all buffers to avoid bugs
    VertexBuffer->Unbind();
    VertexArray->Unbind();
    ElementBuffer->Unbind();

    // Main loop (Game loop)
    while (!MainWindow.Close())
    {
        // Process all window's events and clear all buffers
        MainWindow.ProcessEvents();
        MainWindow.ClearBuffers();

        // Informs OpenGL which shader program and VAO we want to use
        DefaultProgram->Activate();
        VertexArray->Bind();

        // Draw a triangle using the EBO set-up
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(Indices.size()), GL_UNSIGNED_INT, nullptr);

        // Swaps window's buffers
        MainWindow.SwapBuffers();
    }

    // Deletes what we need anymore
    delete DefaultProgram;
    delete VertexBuffer;
    delete VertexArray;
    delete ElementBuffer;

    return 0;
}

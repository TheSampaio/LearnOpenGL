// Pre-compiled headers
#include "PCH.h"

// Project includes
#include "Window.h"

#include "Texture.h"
#include "Shader.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

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
    const std::vector<GLfloat> Vertices
    {
        // === Positions       // === Colours         // === UVs      // === Indices
        -0.8f, -0.8f,  0.0f,    1.0f,  1.0f,  1.0f,    0.0f, 0.0f,    //  0
         0.8f, -0.8f,  0.0f,    1.0f,  1.0f,  1.0f,    1.0f, 0.0f,    //  1
        -0.8f,  0.8f,  0.0f,    1.0f,  1.0f,  1.0f,    0.0f, 1.0f,    //  2
         0.8f,  0.8f,  0.0f,    1.0f,  1.0f,  1.0f,    1.0f, 1.0f,    //  3
    };

    // Indices's dynamic array
    const std::vector<GLuint> Indices
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

    // Loads and creates a texture
    Texture* GigaChad = new Texture("../../Resources/Textures/_gigashad-meme.png");

    // Set-ups VAO's layouts
    VertexArray->AttribPointer(0, 3, 8 * sizeof(GLfloat), 0);                   // Position
    VertexArray->AttribPointer(1, 3, 8 * sizeof(GLfloat), 3 * sizeof(GLfloat)); // Color
    VertexArray->AttribPointer(3, 2, 8 * sizeof(GLfloat), 6 * sizeof(GLfloat)); // UV

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

        // Set-ups texture's uniform and binds the texture
        GigaChad->SetUniform(*DefaultProgram, "DiffuseSampler", 0);
        GigaChad->Bind();

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

    // Deleres the generated texture
    delete GigaChad;

    return 0;
}

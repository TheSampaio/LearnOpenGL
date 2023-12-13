// Pre-compiled headers
#include "PCH.h"

// Project includes
#include "Window.h"
#include "Renderer.h"

int main()
{
    // Gets the window's and renderer's static instance references
    Window& Window = Window::GetInstance();
    Renderer& Renderer = Renderer::GetInstance();

    // Set-ups the window
    Window.SetSize(800, 600);
    Window.SetTitle("Window | OpenGL");
    Window.SetVerticalSynchronization(true);
    Window.SetBackgroundColour(0.2f, 0.2f, 0.4f);

    // Creates the window
    Window.Create();

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
    VertexArray->AttribPointer(1, 3, 8 * sizeof(GLfloat), 3 * sizeof(GLfloat)); // Colour
    VertexArray->AttribPointer(3, 2, 8 * sizeof(GLfloat), 6 * sizeof(GLfloat)); // UV

    // Unbinds all buffers to avoid bugs
    VertexBuffer->Unbind();
    VertexArray->Unbind();
    ElementBuffer->Unbind();

    // Main loop (Game loop)
    while (!Window.Close())
    {
        // Process all window's events and clear all buffers
        Window.ProcessEvents();
        Window.ClearBuffers();

        // Draw call scope (This scope is JUST to organize!)
        {
            // Informs OpenGL which shader program and VAO we want to use
            DefaultProgram->Bind();
            VertexArray->Bind();

            // Set-ups texture's uniform and binds the texture
            Renderer.SetUniform1i(*DefaultProgram, "DiffuseSampler", 0);
            GigaChad->Bind();

            // Draw call command using indices
            Renderer.Draw(Indices);

            // Swaps window's buffers
            Window.SwapBuffers();

            // Unbind everything binded to avoid bugs
            GigaChad->Unbind();
            VertexArray->Unbind();
            DefaultProgram->Unbind();
        }
    }

    // Deletes what we need anymore
    delete VertexBuffer;
    delete VertexArray;
    delete ElementBuffer;
    delete GigaChad;
    delete DefaultProgram;

    return 0;
}

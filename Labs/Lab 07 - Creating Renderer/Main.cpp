// Pre-compiled headers
#include "PCH.h"

// Core
#include "Window.h"
#include "Renderer.h"

int main()
{
    // Gets the window's and renderer's static instance references
    Window& window = Window::GetInstance();
    Renderer& renderer = Renderer::GetInstance();

    // Set-ups the window
    window.SetSize(800, 600);
    window.SetTitle("Window | OpenGL");
    window.SetVerticalSynchronization(true);
    window.SetBackgroundColour(0.2f, 0.2f, 0.4f);

    // Creates the window
    window.Create();

    // Vertices's dynamic array
    const std::vector<GLfloat> vertices
    {
        // === Positions       // === Colours         // === UVs      // === Indices
        -0.8f, -0.8f,  0.0f,    1.0f,  1.0f,  1.0f,    0.0f, 0.0f,    //  0
         0.8f, -0.8f,  0.0f,    1.0f,  1.0f,  1.0f,    1.0f, 0.0f,    //  1
        -0.8f,  0.8f,  0.0f,    1.0f,  1.0f,  1.0f,    0.0f, 1.0f,    //  2
         0.8f,  0.8f,  0.0f,    1.0f,  1.0f,  1.0f,    1.0f, 1.0f,    //  3
    };

    // Indices's dynamic array
    const std::vector<GLuint> indices
    {
        0, 1, 2,
        2, 1, 3
    };

    // Creates a shader program using files for the vertex and fragment shaders
    Shader* pShader = new Shader("Default.vert", "Default.frag");

    // Creates all buffer objects
    VAO* pVertexArray = new VAO();
    VBO* pVertexBuffer = new VBO(vertices);
    EBO* pElementBuffer = new EBO(indices);

    // Loads and creates a texture
    Texture* pGigaChadTexture = new Texture("../../Resources/Textures/_gigashad-meme.png");

    // Set-ups VAO's layouts
    pVertexArray->AttribPointer(0, 3, 8 * sizeof(GLfloat), 0);                   // Position
    pVertexArray->AttribPointer(1, 3, 8 * sizeof(GLfloat), 3 * sizeof(GLfloat)); // Colour
    pVertexArray->AttribPointer(3, 2, 8 * sizeof(GLfloat), 6 * sizeof(GLfloat)); // UV

    // Unbinds all buffers to avoid bugs
    pVertexBuffer->Unbind();
    pVertexArray->Unbind();
    pElementBuffer->Unbind();

    // Main loop (Game loop)
    while (!window.Close())
    {
        // Process all window's events and clear all buffers
        window.ProcessEvents();
        window.ClearBuffers();

        // Draw call scope (This scope is JUST to organize!)
        {
            // Informs OpenGL which shader program and VAO we want to use
            pShader->Bind();
            pVertexArray->Bind();

            // Set-ups texture's uniform and binds the texture
            renderer.SetUniform1i(*pShader, "uDiffuseSampler", 0);
            pGigaChadTexture->Bind();

            // Draw call command using indices
            renderer.Draw(indices);

            // Swaps window's buffers
            window.SwapBuffers();

            // Unbind everything binded to avoid bugs
            pGigaChadTexture->Unbind();
            pVertexArray->Unbind();
            pShader->Unbind();
        }
    }

    // Deletes what we need anymore
    delete pVertexBuffer;
    delete pVertexArray;
    delete pElementBuffer;
    delete pGigaChadTexture;
    delete pShader;
}

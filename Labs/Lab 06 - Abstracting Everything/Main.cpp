// Pre-compiled headers
#include "PCH.h"

// Core
#include "Window.h"

#include "Texture.h"
#include "Shader.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

int main()
{
    // Gets the window's instance reference
    Window& window = Window::GetInstance();

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
    Texture* pGigaChadTexture = new Texture("../../Resources/Textures/meme-this-is-fine.png");

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
            glUniform1i(glGetUniformLocation(pShader->GetId(), "uDiffuseSampler"), 0);
            pGigaChadTexture->Bind();

            // Draw a triangle using the EBO set-up
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, nullptr);

            // Unbind everything binded to avoid bugs
            pGigaChadTexture->Unbind();
            pVertexArray->Unbind();
            pShader->Unbind();
        }

        // Swaps window's buffers
        window.SwapBuffers();
    }

    // Deletes what we need anymore
    delete pVertexBuffer;
    delete pVertexArray;
    delete pElementBuffer;
    delete pGigaChadTexture;
    delete pShader;
}

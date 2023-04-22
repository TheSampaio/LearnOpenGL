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
    MainWindow.SetSize(800, 680);
    MainWindow.SetTitle("Window | OpenGL");
    MainWindow.SetVerticalSynchronization(false);
    MainWindow.SetBackgroundColour(0.2f, 0.2f, 0.4f);

    // Creates the window
    MainWindow.Create();

    // Vertices's array
    std::vector<GLfloat> Vertices
    {
        // === Positions       // === Colours         // === UVs      // === Indices
        -0.8f, -0.8f,  0.0f,    1.0f,  1.0f,  1.0f,    0.0f, 0.0f,    //  0
         0.8f, -0.8f,  0.0f,    1.0f,  1.0f,  1.0f,    1.0f, 0.0f,    //  1
        -0.8f,  0.8f,  0.0f,    1.0f,  1.0f,  1.0f,    0.0f, 1.0f,    //  2
         0.8f,  0.8f,  0.0f,    1.0f,  1.0f,  1.0f,    1.0f, 1.0f,    //  3
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
    VertexArray->AttribPointer(0, 3, 8 * sizeof(GLfloat), 0);                   // Position
    VertexArray->AttribPointer(1, 3, 8 * sizeof(GLfloat), 3 * sizeof(GLfloat)); // Color
    VertexArray->AttribPointer(3, 2, 8 * sizeof(GLfloat), 6 * sizeof(GLfloat)); // UV

    // Unbinds all buffers to avoid bugs
    VertexBuffer->Unbind();
    VertexArray->Unbind();
    ElementBuffer->Unbind();

    // ===== Texture Loading ==================================================================================================== //

    // Flips image on load
    stbi_set_flip_vertically_on_load(true);

    // Loads a image from disk
    int Width = 0, Height = 0, Column = 0;
    unsigned char* Bytes = stbi_load("../../Resources/Textures/_gigashad-meme.png", &Width, &Height, &Column, 0);

    // Generates a texture
    GLuint Texture;
    glGenTextures(1, &Texture);

    // Activates the texture slot 0
    glActiveTexture(GL_TEXTURE0);

    // Bind a 2D texture
    glBindTexture(GL_TEXTURE_2D, Texture);

    // Set-ups the minification and magnification filters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Set-ups the texture's wrap behavior
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Set-ups the texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Bytes);

    // Generates a mipmap for the 2D texture
    glGenerateMipmap(GL_TEXTURE_2D);

    // Free the data loaded from disk
    stbi_image_free(Bytes);

    // Unbinds the 2D texture to avoid bugs
    glBindTexture(GL_TEXTURE_2D, 0);

    // ========================================================================================================================== //

    // Main loop (Game loop)
    while (!MainWindow.Close())
    {
        // Process all window's events and clear all buffers
        MainWindow.ProcessEvents();
        MainWindow.ClearBuffers();

        // Informs OpenGL which shader program and VAO we want to use
        DefaultProgram->Activate();
        VertexArray->Bind();

        // Creates a uniform sampler and binds the generated texture
        glUniform1i(glGetUniformLocation(DefaultProgram->GetId(), "DiffuseSampler"), 0);
        glBindTexture(GL_TEXTURE_2D, Texture);

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
    glDeleteTextures(1, &Texture);

    return 0;
}

// Pre-compiled headers
#include "PCH.h"

// Project includes
#include "Renderer.h"
#include "Window.h"
#include "Camera.h"
#include "Timer.h"

int main()
{
    // Gets all the static instance references
    Window& window = Window::GetInstance();
    Renderer& renderer = Renderer::GetInstance();
    Timer& timer = Timer::GetInstance();

    // Set-ups the window
    window.SetSize(1360, 768);
    window.SetTitle("Window | OpenGL");
    window.SetVerticalSynchronization(false);
    window.SetBackgroundColour(0.2f, 0.2f, 0.4f);

    // Creates the window
    window.Create();

    // Vertices's array
    const std::vector<GLfloat> vertices
    {
        // === Positions       // === Colours         // === UVs      // === Indices
        -0.5f,  0.0f,  0.5f,    1.0f,  1.0f,  1.0f,    0.0f, 0.0f,    //  0
        -0.5f,  0.0f, -0.5f,    1.0f,  1.0f,  1.0f,    5.0f, 0.0f,    //  1
         0.5f,  0.0f, -0.5f,    1.0f,  1.0f,  1.0f,    0.0f, 0.0f,    //  2
         0.5f,  0.0f,  0.5f,    1.0f,  1.0f,  1.0f,    5.0f, 0.0f,    //  3
         0.0f,  0.8f,  0.0f,    1.0f,  1.0f,  1.0f,    2.5f, 5.0f     //  4
    };

    // Indices's dynamic array
    const std::vector<GLuint> indices
    {
       0, 1, 2,
       0, 2, 3,
       0, 1, 4,
       1, 2, 4,
       2, 3, 4,
       3, 0, 4
    };

    // Creates a shader program using files for the vertex and fragment shaders
    Shader* pShader = new Shader("DefaultVert.glsl", "DefaultFrag.glsl");

    // Creates all buffer objects
    VAO* pVertexArray = new VAO();
    VBO* pVertexBuffer = new VBO(vertices);
    EBO* pElementBuffer = new EBO(indices);

    // Loads and creates a texture
    Texture* pSandbrickTexture = new Texture("../../Resources/Textures/diffuse-sandbrick-01.png");

    // Set-ups VAO's layouts
    pVertexArray->AttribPointer(0, 3, 8 * sizeof(GLfloat), 0);                   // Position
    pVertexArray->AttribPointer(1, 3, 8 * sizeof(GLfloat), 3 * sizeof(GLfloat)); // Colour
    pVertexArray->AttribPointer(3, 2, 8 * sizeof(GLfloat), 6 * sizeof(GLfloat)); // UV

    // Unbinds all buffers to avoid bugs
    pVertexBuffer->Unbind();
    pVertexArray->Unbind();
    pElementBuffer->Unbind();

    // Creates a ghost camera
    Camera* pCamera = new Camera(80.0f);

    // Main loop (Game loop)
    while (!window.Close())
    {
        // Updates timer's amount and delta times
        timer.Update();

        // Process all window's events and clear all buffers
        window.ProcessEvents();
        window.ClearBuffers();

        // Draw call scope (This scope is JUST to organize!)
        {
            // Informs OpenGL which shader program and VAO we want to use
            pShader->Bind();
            pVertexArray->Bind();

            // Process all camera's events and calculates the VP matrix
            pCamera->ProcessInputs();
            pCamera->Use(*pShader);

            // Creates a model matrix
            glm::mat4 model = glm::mat4(1.0f);

            // Set-ups the model matrix
            model = glm::translate(model, glm::vec3{ 0.0f, 1.0f, 0.0f } *0.25f);
            model = glm::translate(model, glm::vec3{ 0.0f, 0.0f, 1.0f } *-0.15f);
            model = glm::rotate(model, glm::radians(50.0f * timer.GetAmountTime()), glm::vec3{ 0.0f, 1.0f, 0.0f });

            // Send data from CPU to GPU by using uniforms
            renderer.SetUniformMatrix4fv(*pShader, "uModel", model);

            // Set-ups texture's uniform and binds the texture
            renderer.SetUniform1i(*pShader, "uDiffuseSampler", 0);
            pSandbrickTexture->Bind();

            // Draw call command using indices
            renderer.Draw(indices);

            // Unbind everything binded to avoid bugs
            pSandbrickTexture->Unbind();
            pVertexArray->Unbind();
            pShader->Unbind();
        }

        // Swaps window's buffers
        window.SwapBuffers();
    }

    // Deletes what we need anymore
    delete pCamera;
    delete pVertexBuffer;
    delete pVertexArray;
    delete pElementBuffer;
    delete pSandbrickTexture;
    delete pShader;

    return 0;
}

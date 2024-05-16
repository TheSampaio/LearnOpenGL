// Pre-compiled headers
#include "PCH.h"

// Project includes
#include "Renderer.h"
#include "Window.h"
#include "Camera.h"
#include "Timer.h"

#include "LightDirectional.h"

int main()
{
    // Gets all the static instance references
    Window& window = Window::GetInstance();
    Renderer& renderer = Renderer::GetInstance();
    Timer& timer = Timer::GetInstance();

    // Set-ups the window
    window.SetSize(1360, 768);
    window.SetTitle("Window | OpenGL");
    window.SetVerticalSynchronization(true);
    window.SetBackgroundColour(0.2f, 0.2f, 0.4f);

    // Creates the window
    window.Create();

    // Vertices's array
    const std::vector<GLfloat> vertices
    {
       // === Positions       // === Colours        // === Normals        // === UVs
       -0.5f, 0.0f,  0.5f,     0.8f, 0.8f, 0.8f,     0.0f, -1.0f, 0.0f,    0.0f, 0.0f, // Bottom side
       -0.5f, 0.0f, -0.5f,     0.8f, 0.8f, 0.8f,     0.0f, -1.0f, 0.0f,    0.0f, 5.0f, // Bottom side
        0.5f, 0.0f, -0.5f,     0.8f, 0.8f, 0.8f,     0.0f, -1.0f, 0.0f,    5.0f, 5.0f, // Bottom side
        0.5f, 0.0f,  0.5f,     0.8f, 0.8f, 0.8f,     0.0f, -1.0f, 0.0f,    5.0f, 0.0f, // Bottom side

       -0.5f, 0.0f,  0.5f,     0.8f, 0.8f, 0.8f,    -0.8f, 0.5f,  0.0f,    0.0f, 0.0f, // Left Side
       -0.5f, 0.0f, -0.5f,     0.8f, 0.8f, 0.8f,    -0.8f, 0.5f,  0.0f,    5.0f, 0.0f, // Left Side
        0.0f, 0.8f,  0.0f,     1.0f, 1.0f, 1.0f,    -0.8f, 0.5f,  0.0f,    2.5f, 5.0f, // Left Side

       -0.5f, 0.0f, -0.5f,     0.8f, 0.8f, 0.8f,     0.0f, 0.5f, -0.8f,    5.0f, 0.0f, // Back side
        0.5f, 0.0f, -0.5f,     0.8f, 0.8f, 0.8f,     0.0f, 0.5f, -0.8f,    0.0f, 0.0f, // Back side
        0.0f, 0.8f,  0.0f,     1.0f, 1.0f, 1.0f,     0.0f, 0.5f, -0.8f,    2.5f, 5.0f, // Back side

        0.5f, 0.0f, -0.5f,     0.8f, 0.8f, 0.8f,     0.8f, 0.5f,  0.0f,    0.0f, 0.0f, // Right side
        0.5f, 0.0f,  0.5f,     0.8f, 0.8f, 0.8f,     0.8f, 0.5f,  0.0f,    5.0f, 0.0f, // Right side
        0.0f, 0.8f,  0.0f,     1.0f, 1.0f, 1.0f,     0.8f, 0.5f,  0.0f,    2.5f, 5.0f, // Right side

        0.5f, 0.0f,  0.5f,     0.8f, 0.8f, 0.8f,     0.0f, 0.5f,  0.8f,    5.0f, 0.0f, // Front side
       -0.5f, 0.0f,  0.5f,     0.8f, 0.8f, 0.8f,     0.0f, 0.5f,  0.8f,    0.0f, 0.0f, // Front side
        0.0f, 0.8f,  0.0f,     1.0f, 1.0f, 1.0f,     0.0f, 0.5f,  0.8f,    2.5f, 5.0f  // Front side
    };

    // Indices's dynamic array
    const std::vector<GLuint> indices
    {
        0, 1, 2,    // Bottom side
        0, 2, 3,    // Bottom side
        4, 6, 5,    // Left side
        7, 9, 8,    // Back side
        10, 12, 11, // Right side
        13, 15, 14  // Front side
    };

    // Creates a shader program using files for the vertex and fragment shaders
    Shader* pShader = new Shader("DefaultVert.glsl", "DefaultFrag.glsl");

    // Creates all light sources
    LightDirectional* pSun = new LightDirectional(*pShader,
        glm::vec3{ 0.5f, 1.0f, 0.8f },
        glm::vec3{ 0.45f }
    );

    // Creates all buffer objects
    VAO* pVertexArray = new VAO();
    VBO* pVertexBuffer = new VBO(vertices);
    EBO* pElementBuffer = new EBO(indices);

    // Loads and creates a texture
    Texture* pSandbrickTexture = new Texture("../../Resources/Textures/diffuse-sandbrick-01.png");

    // Set-ups VAO's layouts
    pVertexArray->AttribPointer(0, 3, 11 * sizeof(GLfloat), 0);                   // Position
    pVertexArray->AttribPointer(1, 3, 11 * sizeof(GLfloat), 3 * sizeof(GLfloat)); // Colour
    pVertexArray->AttribPointer(2, 3, 11 * sizeof(GLfloat), 6 * sizeof(GLfloat)); // Normal
    pVertexArray->AttribPointer(3, 2, 11 * sizeof(GLfloat), 9 * sizeof(GLfloat)); // UV

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
            // Uses the light sources
            pSun->Use();

            // Informs OpenGL which shader program and VAO we want to use
            pShader->Bind();
            pVertexArray->Bind();

            // Process all camera's events and calculates the VP matrix
            pCamera->Inputs();
            pCamera->Update(*pShader);

            // Creates a model matrix
            glm::mat4 model = glm::mat4(1.0f);

            // Set-ups the model matrix
            model = glm::translate(model, glm::vec3{ 0.0f, 1.0f, 0.0f } * 0.25f);
            model = glm::translate(model, glm::vec3{ 0.0f, 0.0f, 1.0f } * -0.15f);
            model = glm::rotate(model, glm::radians(50.0f * timer.GetAmountTime()), glm::vec3{ 0.0f, 1.0f, 0.0f });

            // Creates a normal matrix
            glm::mat4 normalMatrix = glm::transpose(glm::inverse(model));

            // Send data from CPU to GPU by using uniforms
            renderer.SetUniformMatrix4fv(*pShader, "uModel", model);
            renderer.SetUniformMatrix4fv(*pShader, "uNormalMatrix", normalMatrix);

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
    delete pSun;
    delete pShader;

    return 0;
}

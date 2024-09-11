// Pre-compiled headers
#include "PCH.h"

// Core
#include "Renderer.h"
#include "Window.h"
#include "Camera.h"
#include "Timer.h"

// Components
#include "Geometry.h"
#include "Material.h"
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
    window.SetVerticalSynchronization(false);
    window.SetBackgroundColour(0.2f, 0.2f, 0.4f);

    // Creates the window
    window.Create();

    // Creates a shader program using files for the vertex and fragment shaders
    Shader* pShader = new Shader("Default.vert", "Default.frag");

    // Creates all light sources
    LightDirectional* pSun = new LightDirectional(glm::vec3{ 1.5f, 1.0, -1.5 },
        glm::vec3{ 0.6f },
        glm::vec3{ 0.8f });

    // Creates all buffer objects
    VAO* pVertexArray = new VAO();
    VBO* pVertexBuffer = new VBO(Geometry::plane.vertices);
    EBO* pElementBuffer = new EBO(Geometry::plane.indices);

    // Loads and creates a texture
    Texture* pSandbrickTexture01 = new Texture("../../Resources/Textures/diffuse-wood-01.png");
    Texture* pSandbrickTexture02 = new Texture("../../Resources/Textures/specular-wood-01.jpg", GL_RED, GL_TEXTURE1);

    // Creates materials
    Material* pGroundMaterial = new Material(pSandbrickTexture01, pSandbrickTexture02, 0.8f, 25.0f);

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
            // Informs OpenGL which shader program and VAO we want to use
            pShader->Bind();
            pVertexArray->Bind();

            // Process all camera's events and calculates the VP matrix
            pCamera->ProcessInputs();
            pCamera->Use(*pShader);

            // Binds the light sources
            pSun->Use(*pShader);

            // Creates a model matrix
            glm::mat4 model = glm::mat4{ 1.0f };

            // Set-ups the model matrix
            model = glm::scale(model, glm::vec3{ 1.0f } * 10.0f);

            // Creates a normal matrix
            glm::mat4 normalMatrix = glm::transpose(glm::inverse(model));

            // Send data from CPU to GPU by using uniforms
            renderer.SetUniformMatrix4fv(*pShader, "uModel", model);
            renderer.SetUniformMatrix4fv(*pShader, "uNormalMatrix", normalMatrix);

            pGroundMaterial->Bind(*pShader);

            // Draw call command using indices
            renderer.Draw(Geometry::plane.indices);

            // Unbind everything binded to avoid bugs
            pGroundMaterial->Unbind();
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
    delete pGroundMaterial;
    delete pSandbrickTexture02;
    delete pSandbrickTexture01;
    delete pSun;
    delete pShader;
}

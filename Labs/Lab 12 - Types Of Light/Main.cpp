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
#include "LightPoint.h"
#include "LightSpot.h"

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

    // Creates a shader program using files for the vertex and fragment shaders
    Shader* pShader = new Shader("Default.vert", "Default.frag");

    std::vector<LightPoint*> lightPoints;

    // Creates all light sources
    LightDirectional* pSun = new LightDirectional(glm::vec3{ 1.5f, 1.0, -1.5 },
        glm::vec3{ 0.2f },
        glm::vec3{ 0.8f },
        glm::vec3{ 0.8f });

    LightPoint* pLamp01 = new LightPoint(glm::vec3{ -3.0f, 1.0f, 0.0f },
        0.2f, 0.15f, 0.1f,
        glm::vec3{ 0.1f },
        glm::vec3{ 0.8f, 0.0f, 0.0f },
        glm::vec3{ 0.6f, 0.0f, 0.0f });

    LightPoint* pLamp02 = new LightPoint(glm::vec3{ 0.0f, 1.0f, -3.0f },
        0.2f, 0.15f, 0.1f,
        glm::vec3{ 0.1f },
        glm::vec3{ 0.0f, 0.8f, 0.0f },
        glm::vec3{ 0.0f, 0.6f, 0.0f });

    LightPoint* pLamp03 = new LightPoint(glm::vec3{ 3.0f, 1.0f, 0.0f },
        0.2f, 0.15f, 0.1f,
        glm::vec3{ 0.1f },
        glm::vec3{ 0.0f, 0.0f, 0.8f },
        glm::vec3{ 0.0f, 0.0f, 0.6f });

    LightPoint* pLamp04 = new LightPoint(glm::vec3{ 0.0f, 1.0f, 3.0f },
        0.2f, 0.15f, 0.1f,
        glm::vec3{ 0.1f },
        glm::vec3{ 0.8f, 0.8f, 0.0f },
        glm::vec3{ 0.6f, 0.6f, 0.0f });

    LightSpot* pSpot = new LightSpot(glm::vec3{ 0.0f, 6.0f, 0.0f },
        glm::vec3{ 0.0f, -1.0f, 0.0f },
        0.02f, 0.96f,
        glm::vec3{ 0.0f },
        glm::vec3{ 1.4f });

    // Store all point lights into an array
    lightPoints.push_back(pLamp01);
    lightPoints.push_back(pLamp02);
    lightPoints.push_back(pLamp03);
    lightPoints.push_back(pLamp04);

    // ===== Plane's Mesh ================================================================================================ //

    // Creates all buffer objects
    VAO* pVertexArrayPlane = new VAO();
    VBO* pVertexBufferPlane = new VBO(Geometry::plane.vertices);
    EBO* pElementBufferPlane = new EBO(Geometry::plane.indices);

    // Loads and creates a texture
    Texture* pTextureDiffusePlane = new Texture("../../Resources/Textures/diffuse-wood-01.png");
    Texture* pTextureSpecularPlane = new Texture("../../Resources/Textures/specular-wood-01.jpg", GL_RED, GL_TEXTURE1);

    // Creates materials
    Material* pMaterialPlane = new Material(pTextureDiffusePlane, pTextureSpecularPlane, 0.8f, 25.0f);

    // Set-ups VAO's layouts
    pVertexArrayPlane->AttribPointer(0, 3, 11 * sizeof(GLfloat), 0);                   // Position
    pVertexArrayPlane->AttribPointer(1, 3, 11 * sizeof(GLfloat), 3 * sizeof(GLfloat)); // Colour
    pVertexArrayPlane->AttribPointer(2, 3, 11 * sizeof(GLfloat), 6 * sizeof(GLfloat)); // Normal
    pVertexArrayPlane->AttribPointer(3, 2, 11 * sizeof(GLfloat), 9 * sizeof(GLfloat)); // UV

    // Unbinds all buffers to avoid bugs
    pVertexBufferPlane->Unbind();
    pVertexArrayPlane->Unbind();
    pElementBufferPlane->Unbind();

    // ===== Pyramid's Mesh ================================================================================================= //

    // Creates all buffer objects
    VAO* pVertexArrayPyramid = new VAO();
    VBO* pVertexBufferPyramid = new VBO(Geometry::pyramid.vertices);
    EBO* pElementBufferPyramid = new EBO(Geometry::pyramid.indices);

    // Loads and creates a texture
    Texture* pTextureDiffusePyramid = new Texture("../../Resources/Textures/diffuse-sandbrick-01.png");
    Texture* pTextureSpecularPyramid = new Texture("../../Resources/Textures/specular-sandbrick-01.jpg", GL_RED, GL_TEXTURE1);

    // Creates materials
    Material* pMaterialPyramid = new Material(pTextureDiffusePyramid, pTextureSpecularPyramid, 0.2f, 15.0f);

    // Set-ups VAO's layouts
    pVertexArrayPyramid->AttribPointer(0, 3, 11 * sizeof(GLfloat), 0);                   // Position
    pVertexArrayPyramid->AttribPointer(1, 3, 11 * sizeof(GLfloat), 3 * sizeof(GLfloat)); // Colour
    pVertexArrayPyramid->AttribPointer(2, 3, 11 * sizeof(GLfloat), 6 * sizeof(GLfloat)); // Normal
    pVertexArrayPyramid->AttribPointer(3, 2, 11 * sizeof(GLfloat), 9 * sizeof(GLfloat)); // UV

    // Unbinds all buffers to avoid bugs
    pVertexBufferPyramid->Unbind();
    pVertexArrayPyramid->Unbind();
    pElementBufferPyramid->Unbind();

    // Creates a ghost camera
    Camera* pCamera = new Camera{ glm::vec3{ 0.0f, 0.4f, 2.0f } };

    // Main loop (Game loop)
    while (!window.Close())
    {
        // Updates timer's amount and delta times
        timer.Update();

        // Creates the model and normal matrixes (Plane)
        glm::mat4 modelPlane = glm::mat4{ 1.0f };
        glm::mat4 normalMatrixPlane = glm::mat4{ 1.0f };

        // Creates the model and normal matrixes (Pyramid)
        glm::mat4 modelPyramid = glm::mat4{ 1.0f };
        glm::mat4 normalMatrixPyramid = glm::mat4{ 1.0f };

        // Update scope (Just to organize)
        {
            // Process all window's events
            window.ProcessEvents();

            // Process all camera's events
            pCamera->ProcessInputs();

            // Set-ups the model matrix (Plane)
            modelPlane = glm::scale(modelPlane, glm::vec3{ 1.0f } * 10.0f);

            // Set-ups the model matrix (Pyramid)
            modelPyramid = glm::translate(modelPyramid, glm::vec3{ 0.0f, 1.0f, 0.0f } * 0.005f);
            modelPyramid = glm::scale(modelPyramid, glm::vec3{ 1.0f } * 2.0f);
        }

        // Clears window's buffers
        window.ClearBuffers();

        // Draw scope (Just to organize)
        {
            // Informs OpenGL which shader program
            pShader->Bind();

            // Calculates the camera's view projection matrix
            pCamera->Use(*pShader);

            // ===== Plane's Mesh ================================================================================================ //

            // Binds the plane VAO
            pVertexArrayPlane->Bind();

            // Binds the plane material
            pMaterialPlane->Bind(*pShader);

            // Updates a normal matrix
            normalMatrixPlane = glm::transpose(glm::inverse(modelPlane));

            // Send data from CPU to GPU by using uniforms
            renderer.SetUniformMatrix4fv(*pShader, "uModel", modelPlane);
            renderer.SetUniformMatrix4fv(*pShader, "uNormalMatrix", normalMatrixPlane);

            // Draw call command using indices
            renderer.Draw(Geometry::plane.indices);

            // Unbinds the plane's material and VAO
            pMaterialPlane->Unbind();
            pVertexArrayPlane->Unbind();

            // ===== Pyramid's Mesh ================================================================================================= //

            // Binds the pyramid VAO
            pVertexArrayPyramid->Bind();

            // Binds the pyramid material
            pMaterialPyramid->Bind(*pShader);

            // Updates the normal matrix
            normalMatrixPlane = glm::transpose(glm::inverse(modelPyramid));

            // Send data from CPU to GPU by using uniforms
            renderer.SetUniformMatrix4fv(*pShader, "uModel", modelPyramid);
            renderer.SetUniformMatrix4fv(*pShader, "uNormalMatrix", normalMatrixPyramid);

            // Draw call command using indices
            renderer.Draw(Geometry::pyramid.indices);

            // Unbinds the pyramid's material and VAO
            pMaterialPyramid->Unbind();
            pVertexArrayPyramid->Unbind();

            // Calculates all the light sources
            {
                // Directional
                pSun->Use(*pShader);

                // Point
                int maxLightPoints = static_cast<int>(lightPoints.size());
                renderer.SetUniform1i(*pShader, "uMaxLightPoints", maxLightPoints);

                for (int i = 0; i < maxLightPoints; i++)
                    lightPoints.at(i)->Use(*pShader, i);

                // Spot
                pSpot->Use(*pShader);
            }
        }

        // Unbinds the shader program
        pShader->Unbind();

        // Swaps window's buffers
        window.SwapBuffers();
    }

    // Deletes what we need anymore
    delete pCamera;

    // Pyramid
    delete pMaterialPyramid;
    delete pTextureSpecularPyramid;
    delete pTextureDiffusePyramid;

    delete pElementBufferPyramid;
    delete pVertexBufferPyramid;
    delete pVertexArrayPyramid;

    // Plane
    delete pMaterialPlane;
    delete pTextureSpecularPlane;
    delete pTextureDiffusePlane;

    delete pElementBufferPlane;
    delete pVertexBufferPlane;
    delete pVertexArrayPlane;

    // Lights
    delete pSpot;
    delete pLamp04;
    delete pLamp03;
    delete pLamp02;
    delete pLamp01;
    delete pSun;

    delete pShader;
}

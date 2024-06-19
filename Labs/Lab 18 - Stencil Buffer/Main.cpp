// Pre-compiled headers
#include "PCH.h"

// Core
#include "Debug.h"
#include "Renderer.h"
#include "Window.h"
#include "Camera.h"
#include "Timer.h"

// Components
#include "Geometry.h"
#include "Material.h"
#include "Mesh.h"
#include "Shape.h"
#include "BufferStencil.h"

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
    window.SetTitle("Window");
    window.SetAntiAliasing(true);
    window.SetVerticalSynchronization(true);
    window.SetBackgroundColour(0.2f, 0.2f, 0.4f);

    // Creates the window
    window.Create();

    // Enables face culling
    renderer.SetFaceCulling(true);

    // Creates a shader program using files for the vertex and fragment shaders
    Shader* pShaderDefault = new Shader("Default.vert", "Default.frag");

    // Creates a shader program using files for the outiline effect
    Shader* pShaderOutline = new Shader("Outline.vert", "Outline.frag");

    // Store all point lights
    std::vector<LightPoint*> lightPoints;

    // Creates all light sources
    LightPoint* pLamp01 = new LightPoint(glm::vec3{ -2.5f, 1.0f, 0.0f },
        0.2f, 0.15f, 0.1f,
        glm::vec3{ 0.2f },
        glm::vec3{ 1.0f, 0.0f, 0.0f },
        glm::vec3{ 0.6f, 0.0f, 0.0f });

    LightPoint* pLamp02 = new LightPoint(glm::vec3{ 0.0f, 1.0f, -2.5f },
        0.2f, 0.15f, 0.1f,
        glm::vec3{ 0.2f },
        glm::vec3{ 0.0f, 1.0f, 0.0f },
        glm::vec3{ 0.0f, 0.6f, 0.0f });

    LightPoint* pLamp03 = new LightPoint(glm::vec3{ 2.5f, 1.0f, 0.0f },
        0.2f, 0.15f, 0.1f,
        glm::vec3{ 0.2f },
        glm::vec3{ 0.0f, 0.0f, 1.0f },
        glm::vec3{ 0.0f, 0.0f, 0.6f });

    LightPoint* pLamp04 = new LightPoint(glm::vec3{ 0.0f, 1.0f, 2.5f },
        0.2f, 0.15f, 0.1f,
        glm::vec3{ 0.2f },
        glm::vec3{ 1.0f, 1.0f, 0.0f },
        glm::vec3{ 0.6f, 0.6f, 0.0f });

    // Store all point lights into an array
    lightPoints.push_back(pLamp01);
    lightPoints.push_back(pLamp02);
    lightPoints.push_back(pLamp03);
    lightPoints.push_back(pLamp04);

    // Loads and creates a texture
    Texture* pTextureDiffusePyramid = new Texture("../../Resources/Textures/diffuse-sandbrick-cartoon-01.png");
    Texture* pTextureSpecularPyramid = new Texture("../../Resources/Textures/specular-sandbrick-01.jpg", GL_RED, GL_TEXTURE1);

    // Create a material
    Material* pMaterialPyramid = new Material(pTextureDiffusePyramid, pTextureSpecularPyramid, 0.2f, 15.0f);

    // Creates a mesh
    Mesh* pMeshPyramid = new Mesh(Geometry::pyramid, pMaterialPyramid);

    // Creates and setup the transform component
    Transform* pTransformPyramid = new Transform();
    pTransformPyramid->Translate(glm::vec3{ 0.0f, 1.0f, 0.0f } * 0.0002f);
    pTransformPyramid->Scale(glm::vec3{ 1.0f } * 2.0f);

    // Creates a ghost camera
    Camera* pCamera = new Camera{ glm::vec3{ 0.0f, 0.4f, 2.0f } };

    // Creates a stencil buffer
    BufferStencil* pBufferStencil = new BufferStencil(GL_KEEP, GL_KEEP, GL_REPLACE);

    float totalTime = 0.0f;
    unsigned frameCount = 0;

    // Main loop (Game loop)
    while (!window.Close())
    {
        // Updates timer's amount and delta times
        timer.Update();

        // Calculates FPS and MS
        totalTime += timer.GetDeltaTime();
        frameCount++;
            
        if (totalTime >= 1.0f)
        {
            std::stringstream text;
            text << std::fixed;
            text.precision(1);

            text << Window::GetInstance().GetTitle().c_str() << " | FPS: " << frameCount << " | MS: " << timer.GetDeltaTime() * 1000 << " | OpenGL";
            Window::GetInstance().SetTitle(text.str().c_str());

            frameCount = 0;
            totalTime -= 1.0f;
        }

        // Update scope (Just to organize)
        {
            // Process all window's events
            window.ProcessEvents();

            // Process all camera's events
            pCamera->ProcessInputs();

            // Rotates the pyramid every frame
            pTransformPyramid->Rotate(glm::vec3{ 0.0f, 50.0f, 0.0f } * timer.GetDeltaTime());
        }

        // Clears window's buffers
        window.ClearBuffers();

        // Draw scope (Just to organize)
        {
            pBufferStencil->Begin(GL_ALWAYS, 1, 0xFF);

            // Informs OpenGL which shader program
            pShaderDefault->Bind();

            // Calculates the camera's view projection matrix
            pCamera->Use(*pShaderDefault);

            renderer.SetUniform3f(*pShaderDefault, "uBackgroundColour",
                window.GetBackgroundColour()[0],
                window.GetBackgroundColour()[1],
                window.GetBackgroundColour()[2]);

            // Draw our meshes
            pMeshPyramid->Draw(*pShaderDefault, *pTransformPyramid);

            // Calculates all the light sources
            {
                // Point
                int maxLightPoints = static_cast<int>(lightPoints.size());
                renderer.SetUniform1i(*pShaderDefault, "uMaxLightPoints", maxLightPoints);

                for (int i = 0; i < maxLightPoints; i++)
                    lightPoints.at(i)->Use(*pShaderDefault, i);
            }

            pBufferStencil->Use(*pShaderOutline, *pCamera);

            // Draw our outline meshes
            pMeshPyramid->Draw(*pShaderOutline, *pTransformPyramid);

            pBufferStencil->End(GL_ALWAYS, 0, 0xFF);
        }

        // Unbinds the shader program
        pShaderOutline->Unbind();
        pShaderDefault->Unbind();

        // Swaps window's buffers
        window.SwapBuffers();
    }

    // Deletes what we need anymore
    delete pBufferStencil;
    delete pCamera;

    // Plane
    delete pTransformPyramid;
    delete pMeshPyramid;
    delete pMaterialPyramid;
    delete pTextureSpecularPyramid;
    delete pTextureDiffusePyramid;

    // Lights
    delete pLamp04;
    delete pLamp03;
    delete pLamp02;
    delete pLamp01;

    // Shaders
    delete pShaderOutline;
    delete pShaderDefault;
}

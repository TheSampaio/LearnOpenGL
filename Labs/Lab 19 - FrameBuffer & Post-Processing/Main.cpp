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
#include "BufferFrame.h"

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

    // Creates a shader program using files for the kernel effect
    Shader* pShaderKernel = new Shader("Kernel.vert", "Kernel.frag");

    // Store all point lights
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

    // Loads and creates a texture
    Texture* pTextureDiffusePlane = new Texture("../../Resources/Textures/diffuse-wood-01.png");
    Texture* pTextureSpecularPlane = new Texture("../../Resources/Textures/specular-wood-01.jpg", GL_RED, GL_TEXTURE1);

    // Creates a material
    Material* pMaterialPlane = new Material(pTextureDiffusePlane, pTextureSpecularPlane, 1.0f, 45.0f);

    // Creates a mesh
    Mesh* pMeshPlane = new Mesh(Geometry::plane, pMaterialPlane);

    // Creates and setup the transform component
    Transform* pTransformPlane = new Transform();
    pTransformPlane->Scale(glm::vec3{ 1.0f } * 10.0f);

    // ===== Pyramid's Mesh ============================================================================================== //

    // Loads and creates a texture
    Texture* pTextureDiffusePyramid = new Texture("../../Resources/Textures/diffuse-sandbrick-01.png");
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

    // Creates a framebuffer
    BufferFrame* pBufferFrame = new BufferFrame();

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

        // Binds the framebuffer
        pBufferFrame->Bind();

        // Clears window's buffers
        window.ClearBuffers();

        // Draw scope (Just to organize)
        {
            // Informs OpenGL which shader program
            pShaderDefault->Bind();

            // Calculates the camera's view projection matrix
            pCamera->Use(*pShaderDefault);

            renderer.SetUniform3f(*pShaderDefault, "uBackgroundColour",
                window.GetBackgroundColour()[0],
                window.GetBackgroundColour()[1],
                window.GetBackgroundColour()[2]);

            // Draw our meshes
            pMeshPlane->Draw(*pShaderDefault, *pTransformPlane);
            pMeshPyramid->Draw(*pShaderDefault, *pTransformPyramid);

            // Calculates all the light sources
            {
                // Directional
                pSun->Use(*pShaderDefault);

                // Point
                int maxLightPoints = static_cast<int>(lightPoints.size());
                renderer.SetUniform1i(*pShaderDefault, "uMaxLightPoints", maxLightPoints);

                for (int i = 0; i < maxLightPoints; i++)
                    lightPoints.at(i)->Use(*pShaderDefault, i);

                // Spot
                pSpot->Use(*pShaderDefault);
            }

            // Binds the default framebuffer
            pBufferFrame->Unbind();

            // Activates the framebuffer
            pBufferFrame->Use(*pShaderKernel);
        }

        // Unbinds the shader program
        pShaderKernel->Unbind();
        pShaderDefault->Unbind();

        // Swaps window's buffers
        window.SwapBuffers();
    }

    // Deletes what we need anymore
    delete pBufferFrame;
    delete pCamera;

    // Plane
    delete pTransformPyramid;
    delete pMeshPyramid;
    delete pMaterialPyramid;
    delete pTextureSpecularPyramid;
    delete pTextureDiffusePyramid;

    // Plane
    delete pTransformPlane;
    delete pMeshPlane;
    delete pMaterialPlane;
    delete pTextureSpecularPlane;
    delete pTextureDiffusePlane;

    // Lights
    delete pSpot;
    delete pLamp04;
    delete pLamp03;
    delete pLamp02;
    delete pLamp01;
    delete pSun;

    // Shaders
    delete pShaderKernel;
    delete pShaderDefault;
}

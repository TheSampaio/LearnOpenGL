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
#include "Skybox.h"
#include "BufferFrame.h"
#include "LightDirectional.h"
#include "ShadowMap.h"

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
    window.SetVerticalSynchronization(false);
    window.SetBackgroundColour(0.2f, 0.2f, 0.2f);

    // Creates the window
    window.Create();

    // Enables face culling
    renderer.SetFaceCulling(true);
    renderer.SetGammaCorrection(true);

    // Creates a shader program using files for the vertex and fragment shaders
    Shader* pShaderDefault = new Shader("Default.vert", "Default.frag", "Default.geom");
    Shader* pShaderSkybox = new Shader("Skybox.vert", "Skybox.frag");
    Shader* pShaderShadow = new Shader("Shadow.vert", "Shadow.frag");

    // All the faces of the cubemap (make sure they are in this exact order)
    std::array<std::string, 6> cubeMapTexture =
    {
        "../../Resources/Textures/skybox-right.jpg",
        "../../Resources/Textures/skybox-left.jpg",
        "../../Resources/Textures/skybox-top.jpg",
        "../../Resources/Textures/skybox-bottom.jpg",
        "../../Resources/Textures/skybox-front.jpg",
        "../../Resources/Textures/skybox-back.jpg"
    };

    // Creates a skybox
    Skybox* pSkybox = new Skybox(cubeMapTexture);

    // Creates all light sources
    LightDirectional* pSun = new LightDirectional(glm::vec3{ 0.5f, 1.0, -1.5 });

    // ===== Plane's Mesh ================================================================================================ //

    // Loads and creates a texture
    Texture* pTextureDiffusePlane = new Texture("../../Resources/Textures/diffuse-sand-01.png");
    Texture* pTextureSpecularPlane = new Texture("../../Resources/Textures/specular-sand-01.jpg", GL_SRGB, GL_RED, GL_TEXTURE1);

    // Creates a material
    Material* pMaterialPlane = new Material(pTextureDiffusePlane, pTextureSpecularPlane, 0.2f, 4.0f);

    // Creates a mesh
    Mesh* pMeshPlane = new Mesh(Geometry::plane, pMaterialPlane);

    // Creates and setup the transform component
    Transform* pTransformPlane = new Transform();
    pTransformPlane->Scale(glm::vec3{ 10.0f });

    // ===== Pyramid's Mesh ============================================================================================== //

    // Loads and creates a texture
    Texture* pTextureDiffusePyramid = new Texture("../../Resources/Textures/diffuse-sandbrick-01.png");
    Texture* pTextureSpecularPyramid = new Texture("../../Resources/Textures/specular-sandbrick-01.jpg", GL_SRGB, GL_RED, GL_TEXTURE1);

    // Create a material
    Material* pMaterialPyramid = new Material(pTextureDiffusePyramid, pTextureSpecularPyramid, 0.2f, 14.0f);

    // Creates a mesh
    Mesh* pMeshPyramid = new Mesh(Geometry::pyramid, pMaterialPyramid);

    // Creates and setup the transform component
    Transform* pTransformPyramid01 = new Transform();
    pTransformPyramid01->Translate(glm::vec3{ 0.0f, 0.0002f, 0.0f });
    pTransformPyramid01->Scale(glm::vec3{ 2.0f });

    // =================================================================================================================== //

    // Creates a ghost camera
    Camera* pCamera = new Camera{ glm::vec3{ 0.0f, 0.4f, 2.0f } };
    const float gamma = 1.0f;

    // Creates a shadow map
    ShadowMap* pShadowMap = new ShadowMap{ glm::vec3{ 0.5f, 1.0, -1.5 }, 2.0f, 64.0f };

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

            text << window.GetTitle().c_str() << " | FPS: " << frameCount << " | MS: " << timer.GetDeltaTime() * 1000 << " | OpenGL";
            window.SetTitle(text.str().c_str());

            frameCount = 0;
            totalTime -= 1.0f;
        }

        // Update scope (Just to organize)
        {
            // Process all window's events
            window.ProcessEvents();

            // Process all camera's events
            pCamera->ProcessInputs();
        }

        // Clears window's buffers
        window.ClearBuffers(gamma);

        // Draw scope (Just to organize)
        {
            // Calculate the shadows of all scene's objects
            pShadowMap->Begin(*pShaderShadow);
            pMeshPyramid->Draw(*pShaderShadow, *pTransformPyramid01);
            pShadowMap->End();

            // Skybox
            pSkybox->Draw(*pShaderSkybox, *pCamera);

            // Calculates the camera's view projection matrix
            pCamera->Use(*pShaderDefault);

            // Send the fog's colour to the GPU
            renderer.SetUniform3f(*pShaderDefault, "uBackgroundColour",
                window.GetBackgroundColour()[0],
                window.GetBackgroundColour()[1],
                window.GetBackgroundColour()[2]);

            // Draw our meshes
            pMeshPlane->Draw(*pShaderDefault, *pTransformPlane, 2.0f);
            pMeshPyramid->Draw(*pShaderDefault, *pTransformPyramid01, 5.0f);

            // Directional
            pSun->Use(*pShaderDefault);

            // Draw shadow map
            pShadowMap->Draw(*pShaderDefault);
        }

        // Swaps window's buffers
        window.SwapBuffers();
    }

    // Deletes what we need anymore
    delete pShadowMap;
    delete pCamera;

    // Pyramid
    delete pTransformPyramid01;
    delete pMeshPyramid;
    delete pMaterialPyramid;
    delete pTextureSpecularPyramid;
    delete pTextureDiffusePyramid;

    // Ground
    delete pTransformPlane;
    delete pMeshPlane;
    delete pMaterialPlane;
    delete pTextureSpecularPlane;
    delete pTextureDiffusePlane;

    // Lights and Skybox
    delete pSun;
    delete pSkybox;

    // Shaders
    delete pShaderShadow;
    delete pShaderSkybox;
    delete pShaderDefault;
}

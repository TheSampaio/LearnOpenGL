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
#include "ShadowMap.h"

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
    window.SetBackgroundColour(0.67f, 0.78f, 0.85f);

    // Creates the window
    window.Create();

    // Enables face culling
    renderer.SetFaceCulling(true);

    // Creates a shader program using files for the vertex and fragment shaders
    Shader* pShaderDefault = new Shader("Default.vert", "Default.frag");
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

    // Store all point lights
    std::vector<LightPoint*> lightPoints;

    // Creates all light sources
    LightDirectional* pSun = new LightDirectional(glm::vec3{ 0.5f, 1.0, -1.5 });

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
    pTransformPyramid->Translate(glm::vec3{ 0.0f, 0.0002f, 0.0f });
    pTransformPyramid->Scale(glm::vec3{ 2.0f });

    // Creates a shadow map
    ShadowMap* pShadowMap = new ShadowMap();

    // Creates a ghost camera
    Camera* pCamera = new Camera{ glm::vec3{ 0.0f, 0.4f, 2.0f } };

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

            // Rotates the pyramid every frame
            pTransformPyramid->Rotate(glm::vec3{ 0.0f, 50.0f, 0.0f } *timer.GetDeltaTime());
        }

        // Clears window's buffers
        window.ClearBuffers();

        // Draw scope (Just to organize)
        {
            // Calculate the shadows of all scene's objects
            pShadowMap->Begin(*pShaderShadow);
            pMeshPyramid->Draw(*pShaderShadow, *pTransformPyramid);
            pShadowMap->End();

            // Draw skybox
            pSkybox->Draw(*pShaderSkybox, *pCamera);

            // Calculates the camera's view projection matrix
            pCamera->Use(*pShaderDefault);

            // Send the fog's colour to the GPU
            renderer.SetUniform3f(*pShaderDefault, "uBackgroundColour",
                window.GetBackgroundColour()[0],
                window.GetBackgroundColour()[1],
                window.GetBackgroundColour()[2]);

            // Draw our meshes
            pMeshPlane->Draw(*pShaderDefault, *pTransformPlane);
            pMeshPyramid->Draw(*pShaderDefault, *pTransformPyramid);

            // Draw directional
            pSun->Use(*pShaderDefault);

            // Draw shadow map
            pShadowMap->Draw(*pShaderDefault);
        }

        // Swaps window's buffers
        window.SwapBuffers();
    }

    // Deletes what we need anymore
    delete pCamera;
    delete pShadowMap;

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
    delete pSun;

    // Shaders
    delete pShaderShadow;
    delete pShaderSkybox;
    delete pShaderDefault;
}

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
    window.SetVerticalSynchronization(false);
    window.SetBackgroundColour(0.67f, 0.78f, 0.85f);

    // Creates the window
    window.Create();

    // Enables face culling
    renderer.SetFaceCulling(true);

    // Creates a shader program using files for the vertex and fragment shaders
    Shader* pShaderDefault = new Shader("Default.vert", "Default.frag");
    Shader* pShaderSkybox = new Shader("Skybox.vert", "Skybox.frag");

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
    Texture* pTextureDiffusePlane = new Texture("../../Resources/Textures/diffuse-grass-01.png");
    Texture* pTextureSpecularPlane = new Texture("../../Resources/Textures/specular-grass-01.jpg", GL_RED, GL_TEXTURE1);

    // Creates a material
    Material* pMaterialPlane = new Material(pTextureDiffusePlane, pTextureSpecularPlane, 0.2f, 8.0f);

    // Creates a mesh
    Mesh* pMeshPlane = new Mesh(Geometry::plane, pMaterialPlane);

    // Creates and setup the transform component
    Transform* pTransformPlane = new Transform();
    pTransformPlane->Scale(glm::vec3{ 1.0f } * 10.0f);

    // ===== Tree's Mesh ============================================================================================== //

    // Loads and creates a texture
    Texture* pTextureDiffuseTree = new Texture("../../Resources/Textures/diffuse-tree-01.png");

    // Create a material
    Material* pMaterialTree = new Material(pTextureDiffuseTree, nullptr, 0.2f, 15.0f);

    // Creates a mesh
    Mesh* pMeshTree01 = new Mesh(Geometry::plane, pMaterialTree);
    Mesh* pMeshTree02 = new Mesh(Geometry::plane, pMaterialTree);

    // Creates and setup the transform component
    Transform* pTransformTree01 = new Transform();
    pTransformTree01->Translate(glm::vec3{ 0.0f, 1.5f, 0.0f });
    pTransformTree01->Rotate(glm::vec3{ 90.0f, 0.0f, 0.0f });
    pTransformTree01->Scale(glm::vec3{ 3.0f });

    Transform* pTransformTree02 = new Transform();
    pTransformTree02->Translate(glm::vec3{ -0.04f, 1.5f, 0.04f });
    pTransformTree02->Rotate(glm::vec3{ 90.0f, 0.0f, 00.0f });
    pTransformTree02->Rotate(glm::vec3{ 0.0f, 0.0f, 90.0f });
    pTransformTree02->Scale(glm::vec3{ 3.0f });

    // ===== Windows's Mesh =========================================================================================== //

    // Loads and creates a texture
    Texture* pTextureWindows = new Texture("../../Resources/Textures/diffuse-windows-logo-01.png");

    // Create a material
    Material* pMaterialWindows = new Material(pTextureWindows, nullptr, 0.0f, 1.0f);

    // Creates a mesh
    Mesh* pMeshWindows = new Mesh(Geometry::plane, pMaterialWindows);

    std::vector<Transform*> pTransformWindows;

    // Takes care of the information needed to draw the windows
    const GLushort numWindows = 500;

    // Takes care of drawing the windows in the right order
    GLushort orderDraw[numWindows]{};
    float distanceCamera[numWindows]{};

    // Generates all windows
    for (GLuint i = 0; i < numWindows; i++)
    {
        // Creates a Transform component for all the Windows
        pTransformWindows.push_back(new Transform());

        // Random positions
        pTransformWindows.at(i)->Translate(glm::vec3
        {
            -15.0f + static_cast<float>(rand()) / static_cast<float>(RAND_MAX / (15.0f - (-15.0f))),
              1.0f + static_cast<float>(rand()) / static_cast<float>(RAND_MAX / (4.0f - 1.0f)),
            -15.0f + static_cast<float>(rand()) / static_cast<float>(RAND_MAX / (15.0f - (-15.0f)))
        });

        // Random rotations
        const float degrees = static_cast<float>(rand()) / static_cast<float>(RAND_MAX / 1.0f);
        pTransformWindows.at(i)->Rotate(glm::vec3{ 90.0f, 0.0f, 0.0f });
        pTransformWindows.at(i)->Rotate(glm::vec3{ 0.0f, 0.0f, degrees } * 360.0f);

        // Draw order
        orderDraw[i] = i;
    }

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
        }

        // Clears window's buffers
        window.ClearBuffers();

        // Draw scope (Just to organize)
        {
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
            pMeshPlane->Draw(*pShaderDefault, *pTransformPlane);

            // Disable face culling
            renderer.SetFaceCulling(false);

            pMeshTree01->Draw(*pShaderDefault, *pTransformTree01);
            pMeshTree02->Draw(*pShaderDefault, *pTransformTree02);

            // Enable blend
            renderer.SetBlending(true);

            // Get distance from each window to the camera
            for (unsigned int i = 0; i < numWindows; i++)
            {
                distanceCamera[i] = glm::length(pCamera->GetPosition() - pTransformWindows[i]->GetPosition());
            }

            // Sort windows by distance from camera
            std::sort(orderDraw, orderDraw + numWindows, [&distanceCamera](unsigned a, unsigned b)
            {
                double diff = distanceCamera[b] - distanceCamera[a];
                return diff < 0;
            });

            // Draw all windows
            for (unsigned i = 0; i < numWindows; i++)
            {
                pMeshWindows->Draw(*pShaderDefault, *pTransformWindows.at(orderDraw[i]));
            }

            // Disable blending and enable face culling
            renderer.SetBlending(false);
            renderer.SetFaceCulling(true);

            // Directional
            pSun->Use(*pShaderDefault);
        }

        // Swaps window's buffers
        window.SwapBuffers();
    }

    // Deletes what we need anymore
    delete pCamera;

    // Windows
    for (unsigned i = 0; i < pTransformWindows.size(); i++)
        delete pTransformWindows.at(i);

    delete pMeshWindows;
    delete pMaterialWindows;
    delete pTextureWindows;

    // Tree
    delete pTransformTree02;
    delete pMeshTree02;
    delete pTransformTree01;
    delete pMeshTree01;
    delete pMaterialTree;
    delete pTextureDiffuseTree;

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
    delete pShaderSkybox;
    delete pShaderDefault;
}

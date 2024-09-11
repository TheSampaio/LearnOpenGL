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
    window.SetBackgroundColour(0.2f, 0.2f, 0.2f);

    // Creates the window
    window.Create();

    // Enables face culling
    renderer.SetFaceCulling(true);

    // Creates a shader program using files for the vertex and fragment shaders
    Shader* pShaderDefault = new Shader("Default.vert", "Default.frag", "Wind.geom");
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
    pTransformPlane->Scale(glm::vec3{ 32.0f });

    // ===== Tree's Mesh =========================================================================================== //

    // Loads and creates a texture
    Texture* pTextureTree = new Texture("../../Resources/Textures/diffuse-tree-01.png");

    // Create a material
    Material* pMaterialTree = new Material(pTextureTree, nullptr);

    // Creates a mesh
    Mesh* pMeshTree = new Mesh(Geometry::plane, pMaterialTree);

    // Stores all trees transforms
    std::vector<Transform*> pTransformTrees;

    // Takes care of the information needed to draw the trees
    const GLushort amountTrees = 500;

    // Generates all windows
    for (GLuint i = 0; i < amountTrees; i++)
    {
        // Creates a Transform component for all the Windows
        pTransformTrees.push_back(new Transform());

        // Random positions
        pTransformTrees.at(i)->Translate(glm::vec3
        {
            -15.0f + static_cast<float>(rand()) / static_cast<float>(RAND_MAX / (15.0f - (-15.0f))),
              1.5f,
            -15.0f + static_cast<float>(rand()) / static_cast<float>(RAND_MAX / (15.0f - (-15.0f)))
        });

        // Random rotations
        const float degrees = static_cast<float>(rand()) / static_cast<float>(RAND_MAX / 1.0f);
        pTransformTrees.at(i)->Rotate(glm::vec3{ 90.0f, 0.0f, 0.0f });
        pTransformTrees.at(i)->Rotate(glm::vec3{ 0.0f, 0.0f, degrees } * 360.0f);

        pTransformTrees.at(i)->Scale(glm::vec3{ 3.0f });
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

            renderer.SetUniform1f(*pShaderDefault, "uScale", 10.0f);

            // Draw our meshes
            pMeshPlane->Draw(*pShaderDefault, *pTransformPlane);

            renderer.SetUniform1f(*pShaderDefault, "uScale", 0.98f);
            renderer.SetUniform1f(*pShaderDefault, "uTime", timer.GetAmountTime() * 2.0f);
            renderer.SetUniform3f(*pShaderDefault, "uWindDirection", 1.0f, 0.0f, 1.0f);

            // Disable face culling
            renderer.SetFaceCulling(false);

            // Draw all windows
            for (unsigned i = 0; i < amountTrees; i++)
                pMeshTree->Draw(*pShaderDefault, *pTransformTrees.at(i));

            // Enable face culling
            renderer.SetFaceCulling(true);

            // Directional
            pSun->Use(*pShaderDefault);
        }

        // Swaps window's buffers
        window.SwapBuffers();
    }

    // Deletes what we need anymore
    delete pCamera;

    // Trees
    for (unsigned i = 0; i < pTransformTrees.size(); i++)
        delete pTransformTrees.at(i);

    delete pMeshTree;
    delete pMaterialTree;
    delete pTextureTree;

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

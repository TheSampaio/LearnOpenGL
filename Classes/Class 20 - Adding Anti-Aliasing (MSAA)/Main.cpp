#include "Model.h"
#include "Skybox.h"
#include "Framebuffer.h"

// Window's macros
#define WINDOW_TITLE                "My OpenGL Window"   // Window's title
#define WINDOW_BACKGROUND_COLOR     0.80f, 0.80f, 0.82f  // Window's background color (0.08f, 0.14f, 0.18f)

// Camera's macros
#define CAMERA_SPEED                0.08f  // Camera's speed
#define CAMERA_SENSITIVITY          100.0f // Camera's sensitivity
#define CAMERA_FOV                  80.0f  // Camera's field of view
#define CAMERA_MIN_DISTANCE         0.1f   // Camera's view minimum distance
#define CAMERA_MAX_DISTANCE         100.0f // Camera's view maximum distance

// Light's macros
#define LIGHT_COLOR                 1.0f,  1.0f,  1.0f,  1.0f // Light's color (1.0f, 1.0f, 1.0f, 1.0f)
#define LIGHT_POSITION              0.0f,  0.4f,  0.0f        // Light's position (0.5f, 0.5f, 0.5f)

static const unsigned int windowWidth = 1024;
static const unsigned int windowHeight = 720;
static const unsigned int antiAliasSamples = 4;

static Camera MainCamera(windowWidth, windowHeight, CAMERA_SENSITIVITY, CAMERA_SPEED, static_cast<float>(windowWidth) / static_cast<float>(windowHeight), glm::vec3(0.0f, 0.4f, 1.5f)); // Create a camera
static void ProcessCloseWindowInput(GLFWwindow* window) // Funtion that process our key inputs
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // If "Esc" iguals "1"
    {
        glfwSetWindowShouldClose(window, true); // Closes window
    }
}
static void FramebufferSizeCallback(GLFWwindow* window, int width, int height) // Funtion that update our window's viewport
{
    MainCamera.width = width;
    MainCamera.height = height;
    MainCamera.aspectRatio = (static_cast<float>(width) / static_cast<float>(height));
    glViewport(0, 0, width, height); // Creates a viewport to the created window
}

int main(void)
{
    glfwInit(); // Initialize GLFW

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                 // Set OpenGL's version to '3'.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);                 // Set OpenGL's version to 3.'3'
    glfwWindowHint(GLFW_SAMPLES, antiAliasSamples);                // Activate anti-alias (MSAA)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Choose OpenGL's CORE mode (Modern OpenGL)

    if (!glfwInit()) // Verify if GLFW was initialized
    {
        std::cout << "[ERROR]: Failed to initialize GLFW" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, WINDOW_TITLE, NULL, NULL); // Create a window

    if (!window) // Verify if window was created
    {
        std::cout << "[ERROR]: Failed to create window" << std::endl;
        glfwTerminate(); // Finilize GLFW
        return -1;
    }

    double previousTime = 0.0;
    double currentTime = 0.0;
    double elapsedTime = 0.0;
    unsigned int counter = 0;

    glfwSwapInterval(GLFW_TRUE);        // Disable V-Sync if equals 0
    glfwMakeContextCurrent(window);     // Creating a context

    if (!gladLoadGL()) // Loading and verify GLAD
    {
        std::cout << "[ERRO] Failed to load GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }        
    
    // Callbacks
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

    // Hadware's specs
    std::cout << "[SPEC] OpenGL Version:  " << glGetString(GL_VERSION) << std::endl;                  // Gets OpenGL's version
    std::cout << "[SPEC] OpenGL Vendor:   " << glGetString(GL_VENDOR) << std::endl;                   // Gets graphics card's vendor
    std::cout << "[SPEC] OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;                 // Gets renderer version
    std::cout << "[SPEC] GLSL Version:    " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl; // Gets GLSL's version
    std::cout << "---" << std::endl;

    // Shader programs
    Shader frameProgram("framebuffer.vert", "framebuffer.frag"); // Create Shader Program (Framebuffer)
    Shader skyboxProgram("skybox.vert", "skybox.frag");          // Create Shader Program (Skybox)
    Shader defaultProgram("default.vert", "default.frag");       // Create Shader Program (Default)
    Shader foliageProgram("foliage.vert", "foliage.frag");       // Create Shader Program (Foliage)

    // Framebuffer's texture
    frameProgram.Active();
    glUniform1i(glGetUniformLocation(frameProgram.ID, "screenTexture"), 0);

    // Light's color and position (Main's Model)
    skyboxProgram.Active();
    glUniform1i(glGetUniformLocation(defaultProgram.ID, "skybox"), 0);

    // Sky and fog colors
    glClearColor(WINDOW_BACKGROUND_COLOR, 1.0f);
    glm::vec3 skyColor = glm::vec3(WINDOW_BACKGROUND_COLOR);

    // Light's position and color
    glm::vec4 lightColor = glm::vec4(LIGHT_COLOR);  // Set light's color
    glm::vec3 lightPos = glm::vec3(LIGHT_POSITION); // Set light's position
    glm::mat4 lightModel = glm::mat4(1.0f);         // Initialize light's matrix

    // Light's color and position (Main's Model)
    defaultProgram.Active();
    glUniform4f(glGetUniformLocation(defaultProgram.ID, "lightColor"), lightColor.r, lightColor.g, lightColor.b, lightColor.a);
    glUniform3f(glGetUniformLocation(defaultProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
    glUniform3f(glGetUniformLocation(defaultProgram.ID, "fogColor"), skyColor.r, skyColor.g, skyColor.b);

    // Light's color and position (Main's Model)
    foliageProgram.Active();
    glUniform4f(glGetUniformLocation(foliageProgram.ID, "lightColor"), lightColor.r, lightColor.g, lightColor.b, lightColor.a);
    glUniform3f(glGetUniformLocation(foliageProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
    glUniform3f(glGetUniformLocation(foliageProgram.ID, "fogColor"), skyColor.r, skyColor.g, skyColor.b);

    // Activate deplth buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Configure face culling
    glEnable(GL_CULL_FACE); // Active face culling
    glCullFace(GL_FRONT);   // Specify what face we want cull
    glFrontFace(GL_CCW);    // Specify witch the face's order's indices

    // Configure color blend (Transparency)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Skybox skybox;

    Model ground("Models/ground/scene.gltf");   // Create ground's model
    Model grass("Models/grass/scene.gltf");     // Create grass's model
    Model kenku("Models/kenku/scene.gltf");     // Create kenku's model

    // Cubremap's image textures
    std::string textures[6]
    {
        "textures/skybox/right.jpg",
        "textures/skybox/left.jpg",
        "textures/skybox/top.jpg",
        "textures/skybox/bottom.jpg",
        "textures/skybox/front.jpg",
        "textures/skybox/back.jpg",
    };

    skybox.Textures(textures);

    // Window's loop
    while (!glfwWindowShouldClose(window))
    {
        ProcessCloseWindowInput(window);                                               // Process window's shutdown input
        MainCamera.Inputs(window);                                                     // Gets all MainCamera's inputs
        MainCamera.UpdateMatrix(CAMERA_FOV, CAMERA_MIN_DISTANCE, CAMERA_MAX_DISTANCE); // Initialize MainCamera's shaders

        // Get current time
        currentTime = glfwGetTime();
        elapsedTime = currentTime - previousTime;
        counter++;

        if (elapsedTime >= 1.0 / 30.0)
        {
            std::string FPS = std::to_string((1.0 / elapsedTime) * counter);
            std::string MS = std::to_string((elapsedTime / counter) * 1000);
            std::string newTitle = WINDOW_TITLE " | FPS: " + FPS + " | MS: " + MS;

            glfwSetWindowTitle(window, newTitle.c_str());
            previousTime = currentTime;
            counter = 0;
        }

        glClearColor(WINDOW_BACKGROUND_COLOR, 1.0f);                                // Background color
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // Clear front buffer and depth buffer  

        // Default draw
        skybox.Draw(skyboxProgram, MainCamera, CAMERA_FOV); // Draw skybox
        ground.Draw(defaultProgram, MainCamera);            // Draw ground
        kenku.Draw(defaultProgram, MainCamera);             // Draw kenku

        // No face culling draw
        glDisable(GL_CULL_FACE);
        grass.Draw(foliageProgram, MainCamera); // Draw grass
        glEnable(GL_CULL_FACE);
        
        glfwSwapBuffers(window); // Swap buffers
        glfwPollEvents();        // Process window's events
    }

    // Delete what we need anymore
    skyboxProgram.Delete();
    defaultProgram.Delete();
    foliageProgram.Delete();

    glfwDestroyWindow(window); // Destroy window's process
    glfwTerminate();           // Finilize GLFW
    return 0;
}

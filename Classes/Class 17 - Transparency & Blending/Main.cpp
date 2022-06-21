#include "Model.h"

// Window's macros
#define WINDOW_TITLE                "My OpenGL Window"   // Window's title
#define windowWidth                 1024                 // Window's width
#define WINDOW_HEIGHT               720                  // Window's height
#define WINDOW_BACKGROUND_COLOR     0.80f, 0.92f, 0.96f  // Window's background color (0.08f, 0.14f, 0.18f)
                                   
// Camera's macros
#define CAMERA_SPEED                0.08f  // Camera's speed
#define CAMERA_SENSITIVITY          100.0f // Camera's sensitivity
#define CAMERA_FOV                  80.0f  // Camera's field of view
#define CAMERA_MIN_DISTANCE         0.01f  // Camera's view minimum distance
#define CAMERA_MAX_DISTANCE         100.0f // Camera's view maximum distance

// Light's macros
#define LIGHT_COLOR                 1.0f,  1.0f,  1.0f,  1.0f // Light's color (1.0f, 1.0f, 1.0f, 1.0f)
#define LIGHT_POSITION              0.0f,  0.4f,  0.0f        // Light's position (0.5f, 0.5f, 0.5f)

static int WindowWidth = 1024;
static int WindowHeight = 720;

Camera MainCamera(WindowWidth, WindowHeight, CAMERA_SENSITIVITY, CAMERA_SPEED, static_cast<float>(WindowWidth) / static_cast<float>(WindowHeight), glm::vec3(0.0f, 0.4f, 1.5f)); // Create a camera

void ProcessCloseWindowInput(GLFWwindow* window) // Funtion that process our key inputs
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // If "Esc" iguals "1"
    {
        glfwSetWindowShouldClose(window, true); // Closes window
    }
}
void FramebufferSizeCallback(GLFWwindow* window, int width, int height) // Funtion that update our window's viewport
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
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Choose OpenGL's CORE mode (Modern OpenGL)

    if (!glfwInit()) // Verify if GLFW was initialized
    {
        std::cout << "[ERROR]: Failed to initialize GLFW" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(windowWidth, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL); // Create a window

    if (!window) // Verify if window was created
    {
        std::cout << "[ERROR]: Failed to create window" << std::endl;
        glfwTerminate(); // Finilize GLFW
        return -1;
    }

    glfwMakeContextCurrent(window);                                  // Creating a context
    gladLoadGL();                                                    // Loading GLAD
    glViewport(0, 0, windowWidth, WINDOW_HEIGHT);
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback); // Create a viewport

    std::cout << "[SPEC] OpenGL Version:  " << glGetString(GL_VERSION) << std::endl;                               // Gets OpenGL's version
    std::cout << "[SPEC] OpenGL Vendor:   " << glGetString(GL_VENDOR) << std::endl;                                // Gets graphics card's vendor
    std::cout << "[SPEC] OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;                              // Gets renderer version
    std::cout << "[SPEC] GLSL Version:    " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl; // Gets GLSL's version
    std::cout << "---" << std::endl;

    Shader shaderProgram("default.vert", "default.frag");   // Create Shader Program (Main)
    Shader grassProgram("foliage.vert", "foliage.frag");    // Create Shader Program (Foilage)

    glm::vec4 lightColor = glm::vec4(LIGHT_COLOR);     // Set light's color
    glm::vec3 lightPos = glm::vec3(LIGHT_POSITION);    // Set light's position
    glm::mat4 lightModel = glm::mat4(1.0f);            // Initialize light's matrix
    lightModel = glm::translate(lightModel, lightPos); // Assigns light's matrix's transformations

    // Sky and fog colors
    glClearColor(WINDOW_BACKGROUND_COLOR, 1.0f);
    glm::vec3 skyColor = glm::vec3(WINDOW_BACKGROUND_COLOR);

    // Light's color and position (Main's Model)
    shaderProgram.Active();
    glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.r, lightColor.g, lightColor.b, lightColor.a);
    glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
    glUniform3f(glGetUniformLocation(shaderProgram.ID, "fogColor"), skyColor.r, skyColor.g, skyColor.b);

    // Light's color and position (Grass's Model)
    grassProgram.Active();
    glUniform4f(glGetUniformLocation(grassProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    glUniform3f(glGetUniformLocation(grassProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
    glUniform3f(glGetUniformLocation(grassProgram.ID, "fogColor"), skyColor.r, skyColor.g, skyColor.b);

    // Activate deplth buffer
    glEnable(GL_DEPTH_TEST); 
    glDepthFunc(GL_LESS);

    // Configure face culling
    glCullFace(GL_FRONT);    // Specify what face we want cull
    glFrontFace(GL_CCW);      // Specify witch the face's order's indices

    // Configure color blend (Transparency)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Model model("Models/ground/scene.gltf"); // Create main's model
    Model grass("Models/grass/scene.gltf");  // Create grass's model

    double previousTime = 0.0;
    double currentTime = 0.0;
    double elapsedTime;
    unsigned int counter = 0;

    glfwSwapInterval(GLFW_TRUE); // Disable V-Sync if equals 0

    while (!glfwWindowShouldClose(window)) // Windows's loop
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear front buffer and depth buffer
        ProcessCloseWindowInput(window);                    // Process window's shutdown input

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
            std::string newTitle = WINDOW_TITLE " - " + FPS + " fps | " + MS + " ms";

            glfwSetWindowTitle(window, newTitle.c_str());
            previousTime = currentTime;
            counter = 0;
        }

        // --- Face culling ON ------
        glEnable(GL_CULL_FACE);                // Active face culling
        model.Draw(shaderProgram, MainCamera); // Draw model
        glDisable(GL_CULL_FACE);               // Disable face culling

        // --- Face culling OFF ------
        glEnable(GL_BLEND);                     // Active blend color
        grass.Draw(grassProgram, MainCamera);   // Draw grass model
        glDisable(GL_BLEND);                    // Disable blend color

        glfwSwapBuffers(window); // Swap buffers
        glfwPollEvents();        // Process window's events
    }

    // Delete what we need anymore
    shaderProgram.Delete();
    grassProgram.Delete();

    glfwDestroyWindow(window); // Destroy window's process
    glfwTerminate();           // Finilize GLFW
    return 0;
}

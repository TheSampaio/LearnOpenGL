#include "Model.h"

// Window's macros
#define WINDOW_TITLE                "My OpenGL Window"  // Window's title
#define windowWidth                 1024                // Window's width
#define WINDOW_HEIGHT               720                 // Window's height
#define WINDOW_BACKGROUND_COLOR     0.08f, 0.14f, 0.18f // Window's background color (0.08f, 0.14f, 0.18f)
                                   
// Camera's macros
#define CAMERA_SPEED                0.04f  // Camera's speed
#define CAMERA_SENSITIVITY          100.0f // Camera's sensitivity
#define CAMERA_FOV                  80.0f  // Camera's field of view
#define CAMERA_MIN_DISTANCE         0.01f  // Camera's view minimum distance
#define CAMERA_MAX_DISTANCE         100.0f // Camera's view maximum distance

// Light's macros
#define LIGHT_COLOR                 1.0f,  1.0f,  1.0f,  1.0f // Light's color (1.0f, 1.0f, 1.0f, 1.0f)
#define LIGHT_POSITION              0.0f,  0.4f,  0.0f        // Light's position (0.5f, 0.5f, 0.5f)

// Model's macros
#define MODEL_PATH                  "models/kenku/scene.gltf"

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

    std::cout << "OpenGL Version:  " << glGetString(GL_VERSION) << std::endl;                               // Gets OpenGL's version
    std::cout << "OpenGL Vendor:   " << glGetString(GL_VENDOR) << std::endl;                                // Gets graphics card's vendor
    std::cout << "OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;                              // Gets renderer version
    std::cout << "GLSL Version:    " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl << std::endl; // Gets GLSL's version

    Shader shaderProgram("default.vert", "default.frag"); // Create a Shader Program
    Shader outlineProgram("outlining.vert", "outlining.frag");

    glm::vec4 lightColor = glm::vec4(LIGHT_COLOR);     // Set light's color
    glm::vec3 lightPos = glm::vec3(LIGHT_POSITION);    // Set light's position
    glm::mat4 lightModel = glm::mat4(1.0f);            // Initialize light's matrix
    lightModel = glm::translate(lightModel, lightPos); // Assigns light's matrix's transformations

    shaderProgram.Active(); // Active shader program

    // Light's color and position
    glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.r, lightColor.g, lightColor.b, lightColor.a);
    glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

    // Fog color
    glm::vec3 skyColor = glm::vec3(WINDOW_BACKGROUND_COLOR);
    glUniform3f(glGetUniformLocation(shaderProgram.ID, "fogColor"), skyColor.r, skyColor.g, skyColor.b);

    // Sky color
    glClearColor(WINDOW_BACKGROUND_COLOR, 1.0f);

    // Activate deplth buffer
    glEnable(GL_DEPTH_TEST); 
    glDepthFunc(GL_LESS);

    // Activate stencil buffer
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    // Active face culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);    // Specify what face we want cull
    glFrontFace(GL_CW);      // Specify witch the face's order's indices

    Model model(MODEL_PATH); // Create a model

    double previousTime = 0.0;
    double currentTime = 0.0;
    double elapsedTime;
    unsigned int counter = 0;

    glfwSwapInterval(1); // Disable V-Sync if equals 0

    while (!glfwWindowShouldClose(window)) // Windows's loop
    {
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

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // Clear front buffer and depth buffer
        ProcessCloseWindowInput(window);                                            // Process window's shutdown input

        MainCamera.Inputs(window);                                                     // Gets all MainCamera's inputs
        MainCamera.UpdateMatrix(CAMERA_FOV, CAMERA_MIN_DISTANCE, CAMERA_MAX_DISTANCE); // Initialize MainCamera's shaders

        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
        model.Draw(shaderProgram, MainCamera); // Draw the created model

        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);
        glDisable(GL_DEPTH_TEST);

        outlineProgram.Active();
        glUniform1f(glGetUniformLocation(outlineProgram.ID, "outlining"), 0.005f);
        model.Draw(outlineProgram, MainCamera);

        glStencilMask(0xFF);
        glStencilFunc(GL_ALWAYS, 0, 0xFF);
        glEnable(GL_DEPTH_TEST);

        glfwSwapBuffers(window); // Swap buffers
        glfwPollEvents();        // Process window's events
    }

    // Delete what we need anymore
    shaderProgram.Delete();

    glfwDestroyWindow(window); // Destroy window's process
    glfwTerminate();           // Finilize GLFW
    return 0;
}

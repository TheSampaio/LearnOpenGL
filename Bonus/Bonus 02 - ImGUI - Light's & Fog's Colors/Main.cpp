#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include "Model.h"

// --- Define all macros ---------- //

// Window's macros
#define WINDOW_TITLE                "My OpenGL Window"                      // Window's title
#define WINDOW_WIDTH                800                                     // Window's width
#define WINDOW_HEIGHT               (WINDOW_WIDTH/2) + ((WINDOW_WIDTH/2)/2) // Window's height
#define WINDOW_BACKGROUND_COLOR     0.85f,  0.85f,  0.90f,  1.0f            // Window's background color (0.08f, 0.14f, 0.18f, 1.0f)

// Camera's macros
#define CAMERA_FOV                  80.0f              // Camera's field of view
#define CAMERA_SPEED                0.05f              // Camera's speed
#define CAMERA_POSITION             0.0f,  1.1f,  1.5f // Camera's position (0.0f, 0.5f, 1.5f)
#define CAMERA_SENSITIVITY          100.0f             // Camera's sensitivity
#define CAMERA_MIN_DISTANCE         0.01f              // Camera's view minimum distance
#define CAMERA_MAX_DISTANCE         100.0f             // Camera's view maximum distance

// Light's macros
#define LIGHT_COLOR                 0.861f,  0.861f,  0.900f,  1.000f // Light's color (1.0f, 1.0f, 1.0f, 1.0f)
#define LIGHT_POSITION              0.0f,  0.4f,  0.0f                // Light's position (0.5f, 0.5f, 0.5f)

// -------------------------------- //

// Referencing the ProcessInput and FramebufferSizeCallback functions that was created at code's end
void ProcessCloseWindowInput(GLFWwindow* window);
void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

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

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL); // Create a window

    if (!window) // Verify if window was created
    {
        std::cout << "[ERROR]: Failed to create window" << std::endl;
        glfwTerminate(); // Finilize GLFW
        return -1;
    }

    glfwMakeContextCurrent(window);                                  // Creating a context
    gladLoadGL();                                                    // Loading GLAD
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback); // Create a viewport

    std::cout << "Graphics Card " << glGetString(GL_RENDERER) << std::endl;  // Gets graphics card's info
    std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl;           // Gets OpenGL's version

    Shader shaderProgram("default.vert", "default.frag"); // Create a Shader Program

    float color[4] = { LIGHT_COLOR };

    //glm::vec4 lightColor = glm::vec4(color);     // Set light's color
    glm::vec3 lightPos = glm::vec3(LIGHT_POSITION);    // Set light's position
    glm::mat4 lightModel = glm::mat4(1.0f);            // Initialize light's matrix
    lightModel = glm::translate(lightModel, lightPos); // Assigns light's matrix's transformations

    shaderProgram.Active(); // Actives pyramid's shader program
    glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

    glEnable(GL_DEPTH_TEST); // Avoid depth visual glitchs
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);  // Active face culling
    glCullFace(GL_FRONT);    // Specify what face we want cull
    glFrontFace(GL_CW);      // Specify witch the face's order's indices

    Camera camera(WINDOW_WIDTH, WINDOW_HEIGHT, CAMERA_SENSITIVITY, CAMERA_SPEED, glm::vec3(CAMERA_POSITION)); // Create a camera
    Model model("models/terrain/scene.gltf");                                                                 // Create a model

    double previousTime = 0.0;
    double currentTime = 0.0;
    double elapsedTime;
    unsigned int counter = 0;

    glfwSwapInterval(1); // Disable V-Sync if equals 0

    // ImGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

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

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        camera.Inputs(window);           // Gets all camera's inputs
        ProcessCloseWindowInput(window); // Process window's shutdown input

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);                        // Clear front buffer and depth buffer
        camera.UpdateMatrix(CAMERA_FOV, CAMERA_MIN_DISTANCE, CAMERA_MAX_DISTANCE); // Initialize camera's shaders

        model.Draw(shaderProgram, camera); // Draw the created model

        ImGui::Begin("Sun Light");
        ImGui::Text("Color");
        ImGui::ColorEdit4("Color", color);
        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glClearColor(color[0], color[1], color[2], color[3]); // Set window's background's color
        glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), color[0], color[1], color[2], color[3]);

        glfwSwapBuffers(window); // Swapp buffers
        glfwPollEvents();        // Process window's events
    }

    // --- Delete what we don't need anymore ---------- //

        // Plane's model
    shaderProgram.Delete();

    // ------------------------------------------------ //

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window); // Destroy window's process
    glfwTerminate();           // Finilize GLFW
    return 0;
}

void ProcessCloseWindowInput(GLFWwindow* window) // Funtion that process our key inputs
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // If "Esc" iguals "1"
    {
        glfwSetWindowShouldClose(window, true); // Closes window
    }
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height) // Funtion that update our window's viewport
{
    glViewport(0, 0, width, height); // Creates a viewport to the created window
}
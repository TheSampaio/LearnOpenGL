#include "Model.h"

// Window's macros
#define WINDOW_TITLE                "My OpenGL Window"   // Window's title
#define WINDOW_BACKGROUND_COLOR     0.08f, 0.14f, 0.18f  // Window's background color (0.08f, 0.14f, 0.18f)
                                   
// Camera's macros
#define CAMERA_SPEED                0.06f  // Camera's speed
#define CAMERA_SENSITIVITY          100.0f // Camera's sensitivity
#define CAMERA_FOV                  80.0f  // Camera's field of view
#define CAMERA_MIN_DISTANCE         0.01f  // Camera's view minimum distance
#define CAMERA_MAX_DISTANCE         100.0f // Camera's view maximum distance

// Light's macros
#define LIGHT_COLOR                 1.0f,  1.0f,  1.0f,  1.0f // Light's color (1.0f, 1.0f, 1.0f, 1.0f)
#define LIGHT_POSITION              0.0f,  0.4f,  0.0f        // Light's position (0.5f, 0.5f, 0.5f)

static int windowWidth = 1024;
static int windowHeight = 720;

Camera MainCamera(windowWidth, windowHeight, CAMERA_SENSITIVITY, CAMERA_SPEED, static_cast<float>(windowWidth) / static_cast<float>(windowHeight), glm::vec3(0.0f, 0.4f, 1.5f)); // Create a camera

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

float canvasVertices[]
{
    // Coords    // texCoords
     1.0f, -1.0f,  1.0f, 0.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
    -1.0f,  1.0f,  0.0f, 1.0f,

     1.0f,  1.0f,  1.0f, 1.0f,
     1.0f, -1.0f,  1.0f, 0.0f,
    -1.0f,  1.0f,  0.0f, 1.0f
};

int main(void)
{
    if (!glfwInit()) // Initialize and Verify if GLFW was initialized
    {
        std::cout << "[ERROR]: Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                 // Set OpenGL's version to '3'.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);                 // Set OpenGL's version to 3.'3'
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Choose OpenGL's CORE mode (Modern OpenGL)

    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, WINDOW_TITLE, NULL, NULL); // Create a window

    if (!window) // Verify if window was created
    {
        std::cout << "[ERROR]: Failed to create window" << std::endl;
        glfwTerminate(); // Finilize GLFW
        return -1;
    }

    double previousTime = 0.0;
    double currentTime = 0.0;
    double elapsedTime;
    unsigned int counter = 0;

    glfwSwapInterval(GLFW_TRUE);                                     // Disable V-Sync if equals 0
    glfwMakeContextCurrent(window);                                  // Creating a context
    
    if (!gladLoadGL()) // Load and verify glad
    {
        std::cout << "[ERRO] Failed to load glad" << std::endl;
        glfwTerminate();
        return -1;
    }

    std::cout << "[SPEC] OpenGL Version:  " << glGetString(GL_VERSION) << std::endl;                               // Gets OpenGL's version
    std::cout << "[SPEC] OpenGL Vendor:   " << glGetString(GL_VENDOR) << std::endl;                                // Gets graphics card's vendor
    std::cout << "[SPEC] OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;                              // Gets renderer version
    std::cout << "[SPEC] GLSL Version:    " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl; // Gets GLSL's version
    std::cout << "---" << std::endl;

    // --- Callbacks ------
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback); // Resize window's size and aspect ratio

    Shader shaderProgram("default.vert", "default.frag");   // Create Shader Program (Main)
    Shader canvasProgram("canvas.vert", "canvas.frag");

    glm::vec4 lightColor = glm::vec4(LIGHT_COLOR);     // Set light's color
    glm::vec3 lightPos = glm::vec3(LIGHT_POSITION);    // Set light's position

    // Sky and fog colors
    glm::vec3 skyColor = glm::vec3(WINDOW_BACKGROUND_COLOR);

    // Light's color and position (Main's Model)
    shaderProgram.Active();
    glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.r, lightColor.g, lightColor.b, lightColor.a);
    glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
    glUniform3f(glGetUniformLocation(shaderProgram.ID, "fogColor"), skyColor.r, skyColor.g, skyColor.b);

    // Canvas's texture
    canvasProgram.Active();
    glUniform1i(glGetUniformLocation(canvasProgram.ID, "screenTexture"), 0);

    // Activate depth buffer
    glEnable(GL_DEPTH_TEST);

    // Configure face culling
    glEnable(GL_CULL_FACE);  // Active face culling
    glCullFace(GL_FRONT);     // Specify what face we want cull
    glFrontFace(GL_CCW);      // Specify witch the face's order's indices

    Model model("Models/kenku/scene.gltf"); // Create main's model

    // Prepare framebuffer rectangle VBO and VAO
    GLuint canvasVAO, canvasVBO;
    glGenVertexArrays(1, &canvasVAO);
    glGenBuffers(1, &canvasVBO);

    glBindVertexArray(canvasVAO);
    glBindBuffer(GL_ARRAY_BUFFER, canvasVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(canvasVertices), &canvasVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    // Create Frame Buffer Object
    GLuint FBO;
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    // Create Framebuffer Texture
    GLuint framebufferTexture;
    glGenTextures(1, &framebufferTexture);
    glBindTexture(GL_TEXTURE_2D, framebufferTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, MainCamera.width, MainCamera.height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Prevents edge bleeding

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebufferTexture, 0);

    // Create Render Buffer Object
    GLuint RBO;
    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, MainCamera.width, MainCamera.height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

    // Framebuffer and Renderbuffer status
    auto renderBufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (renderBufferStatus != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "[ERRO] Framebuffer error: " << renderBufferStatus << std::endl;

    // Gameloop
    while (!glfwWindowShouldClose(window)) // Windows's loop
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
            std::string newTitle = WINDOW_TITLE " - " + FPS + " fps | " + MS + " ms";

            glfwSetWindowTitle(window, newTitle.c_str());
            previousTime = currentTime;
            counter = 0;
        }

        // Fix framebuffer's aspect ratio
        glBindTexture(GL_TEXTURE_2D, framebufferTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, MainCamera.width, MainCamera.height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

        // Fix renderbuffer's aspect ratio and use it
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, MainCamera.width, MainCamera.height);

        glClearColor(WINDOW_BACKGROUND_COLOR, 1.0f);        // Background color
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear front buffer and depth buffer

        glEnable(GL_DEPTH_TEST);

        model.Draw(shaderProgram, MainCamera); // Draw model

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        canvasProgram.Active();
        glBindVertexArray(canvasVAO);

        glDisable(GL_DEPTH_TEST);
        glBindTexture(GL_TEXTURE_2D, framebufferTexture);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window); // Swap buffers
        glfwPollEvents();        // Process window's events
    }

    // Delete what we need anymore
    shaderProgram.Delete();
    glDeleteFramebuffers(1, &FBO);

    glfwDestroyWindow(window); // Destroy window's process
    glfwTerminate();           // Finilize GLFW
    return 0;
}

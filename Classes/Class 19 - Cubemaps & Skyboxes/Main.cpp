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

static GLfloat skyboxVertices[]
{
    -1.0f, -1.0f, -1.0f, //          4 ---------- 5
     1.0f, -1.0f, -1.0f, //        / |          / |
    -1.0f, -1.0f,  1.0f, //       /  |         /  |
     1.0f, -1.0f,  1.0f, //      6 --|------- 7   |
                         //      |   0 -------|-- 1
    -1.0f,  1.0f, -1.0f, //      |  /         |  /
     1.0f,  1.0f, -1.0f, //      | /          | /
    -1.0f,  1.0f,  1.0f, //      |/           |/
     1.0f,  1.0f,  1.0f  //      2 ---------- 3
};

static GLuint skyboxIndices[]
{
    // Bottom
    0, 3, 2,
    0, 1, 3,

    // Top
    4, 6, 7,
    7, 5, 4,

    // Left
    3, 1, 7,
    1, 5, 7,

    // Right
    0, 2, 6,
    0, 6, 4,

    // Back
    0, 4, 5,
    0, 5, 1,

    // Front
    2, 3, 6,
    3, 7, 6
};

static Camera MainCamera(WindowWidth, WindowHeight, CAMERA_SENSITIVITY, CAMERA_SPEED, static_cast<float>(WindowWidth) / static_cast<float>(WindowHeight), glm::vec3(0.0f, 0.4f, 1.5f)); // Create a camera

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
    std::cout << "[SPEC] OpenGL Version:  " << glGetString(GL_VERSION) << std::endl;                               // Gets OpenGL's version
    std::cout << "[SPEC] OpenGL Vendor:   " << glGetString(GL_VENDOR) << std::endl;                                // Gets graphics card's vendor
    std::cout << "[SPEC] OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;                              // Gets renderer version
    std::cout << "[SPEC] GLSL Version:    " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl; // Gets GLSL's version
    std::cout << "---" << std::endl;

    // Shader programs
    Shader shaderProgram("default.vert", "default.frag");   // Create Shader Program (Main)
    Shader skyboxProgram("skybox.vert", "skybox.frag");     // Create Shader Program (Skybox)

    // Sky and fog colors
    glClearColor(WINDOW_BACKGROUND_COLOR, 1.0f);
    glm::vec3 skyColor = glm::vec3(WINDOW_BACKGROUND_COLOR);

    // Light's position and color
    glm::vec4 lightColor = glm::vec4(LIGHT_COLOR);     // Set light's color
    glm::vec3 lightPos = glm::vec3(LIGHT_POSITION);    // Set light's position
    glm::mat4 lightModel = glm::mat4(1.0f);            // Initialize light's matrix
    lightModel = glm::translate(lightModel, lightPos); // Assigns light's matrix's transformations

    // Light's color and position (Main's Model)
    shaderProgram.Active();
    glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.r, lightColor.g, lightColor.b, lightColor.a);
    glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
    glUniform3f(glGetUniformLocation(shaderProgram.ID, "fogColor"), skyColor.r, skyColor.g, skyColor.b);

    // Light's color and position (Main's Model)
    skyboxProgram.Active();
    glUniform1i(glGetUniformLocation(shaderProgram.ID, "skybox"), 0);

    // Activate deplth buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Configure face culling
    glEnable(GL_CULL_FACE);     // Active face culling
    glCullFace(GL_FRONT);       // Specify what face we want cull
    glFrontFace(GL_CCW);        // Specify witch the face's order's indices

    // Configure color blend (Transparency)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Model model("Models/airplane/scene.gltf"); // Create main's model  

    // Skybox
    GLuint skyVAO, skyVBO, skyEBO;
    glGenVertexArrays(1, &skyVAO);
    glGenBuffers(1, &skyVBO);
    glGenBuffers(1, &skyEBO);

    glBindVertexArray(skyVAO);

    glBindBuffer(GL_ARRAY_BUFFER, skyVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), skyboxIndices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(float), nullptr);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, skyVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyEBO);

    // Cubremap's image textures
    std::string cubemapFaces[6]
    {
        "textures/skybox/right.jpg",
        "textures/skybox/left.jpg",
        "textures/skybox/top.jpg",
        "textures/skybox/bottom.jpg",
        "textures/skybox/front.jpg",
        "textures/skybox/back.jpg",
    };

    // Cubemap's texture
    GLuint cubemapTexture;
    glGenTextures(1, &cubemapTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    for (unsigned int i = 0; i < 6; i++)
    {
        int width, height, numChanels;
        unsigned char* data = stbi_load(cubemapFaces[i].c_str(), &width, &height, &numChanels, 0);

        if (data)
        {
            stbi_set_flip_vertically_on_load(false);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }

        else
        {
            std::cout << "[ERRO] Failed to load cubemap's texture: " << cubemapFaces[i] << std::endl;
            stbi_image_free(data);
        }
    }

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
            std::string newTitle = WINDOW_TITLE " | FPS: " + FPS + " | MS: " + MS;

            glfwSetWindowTitle(window, newTitle.c_str());
            previousTime = currentTime;
            counter = 0;
        }

        model.Draw(shaderProgram, MainCamera); // Draw model

        glDepthFunc(GL_LEQUAL);

        skyboxProgram.Active();
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        
        view = glm::mat4(glm::mat3(glm::lookAt(MainCamera.position, MainCamera.position + MainCamera.orientation, MainCamera.upVector)));
        projection = glm::perspective(glm::radians(CAMERA_FOV), MainCamera.aspectRatio, 0.1f, 100.0f);

        glUniformMatrix4fv(glGetUniformLocation(skyboxProgram.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(skyboxProgram.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(skyVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glDepthFunc(GL_LESS);

        glfwSwapBuffers(window); // Swap buffers
        glfwPollEvents();        // Process window's events
    }

    // Delete what we need anymore
    shaderProgram.Delete();
    skyboxProgram.Delete();

    glfwDestroyWindow(window); // Destroy window's process
    glfwTerminate();           // Finilize GLFW
    return 0;
}

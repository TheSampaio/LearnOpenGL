#include "Mesh.h"

// --- Define all macros ---------- //

// Window's macros
#define WINDOW_NAME                 "My OpenGL Window"                      // Window's title
#define WINDOW_WIDTH                800                                     // Window's width
#define WINDOW_HEIGHT               (WINDOW_WIDTH/2) + ((WINDOW_WIDTH/2)/2) // Window's height
#define WINDOW_BACKGROUND_COLOR     0.08f, 0.14f, 0.18f, 1.0f               // Window's background color (0.08f, 0.14f, 0.18f, 1.0f)
                                   
// Camera's macros
#define CAMERA_SPEED                0.02f  // Camera's speed
#define CAMERA_SENSITIVITY          100.0f // Camera's sensitivity
#define CAMERA_FOV                  80.0f  // Camera's field of view
#define CAMERA_MIN_DISTANCE         0.01f  // Camera's view minimum distance
#define CAMERA_MAX_DISTANCE         100.0f // Camera's view maximum distance

// Light's macros
#define LIGHT_COLOR                 1.0f,  1.0f,  1.0f,  1.0f // Light's color (1.0f, 1.0f, 1.0f, 1.0f)
#define LIGHT_POSITION              0.0f,  0.4f,  0.0f        // Light's position (0.5f, 0.5f, 0.5f)

// Plane's macros
#define PLANE_POSITION            0.0f, 0.0f, 0.0f // Pyramid's position (0.0f, 0.0f, 0.0f)

// -------------------------------- //

// Vertices data array (Plane)
Vertex vertices[]
{ //     COORDINATES                       /           NORMALS            /            COLORS            /        TEXTURES        //
    Vertex{glm::vec3(-1.0f, -0.2f,  1.0f),   glm::vec3(0.0f, 1.0f, 0.0f),   glm::vec3(1.0f, 1.0f, 1.0f),   glm::vec2(0.0f, 0.0f)},
    Vertex{glm::vec3(-1.0f, -0.2f, -1.0f),   glm::vec3(0.0f, 1.0f, 0.0f),   glm::vec3(1.0f, 1.0f, 1.0f),   glm::vec2(0.0f, 1.0f)},
    Vertex{glm::vec3( 1.0f, -0.2f, -1.0f),   glm::vec3(0.0f, 1.0f, 0.0f),   glm::vec3(1.0f, 1.0f, 1.0f),   glm::vec2(1.0f, 1.0f)},
    Vertex{glm::vec3( 1.0f, -0.2f,  1.0f),   glm::vec3(0.0f, 1.0f, 0.0f),   glm::vec3(1.0f, 1.0f, 1.0f),   glm::vec2(1.0f, 0.0f)}
};

// Indices data array (Plane)
GLuint indices[]
{
    0, 1, 2,
    0, 2, 3
};

// Vertices data array (Light)
Vertex lightVertices[]
{ //              COORDINATES              //
    Vertex{glm::vec3(-0.1f, -0.1f,  0.1f)},
    Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
    Vertex{glm::vec3( 0.1f, -0.1f, -0.1f)},
    Vertex{glm::vec3( 0.1f, -0.1f,  0.1f)},
    Vertex{glm::vec3(-0.1f,  0.1f,  0.1f)},
    Vertex{glm::vec3(-0.1f,  0.1f, -0.1f)},
    Vertex{glm::vec3( 0.1f,  0.1f, -0.1f)},
    Vertex{glm::vec3( 0.1f,  0.1f,  0.1f)}
};

// Indices data array (Light)
GLuint lightIndices[]
{
    0, 1, 2,
    0, 2, 3,
    0, 4, 7,
    0, 7, 3,
    3, 7, 6,
    3, 6, 2,
    2, 6, 5,
    2, 5, 1,
    1, 5, 4,
    1, 4, 0,
    4, 5, 6,
    4, 6, 7
};

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

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, NULL, NULL); // Create a window

    if (!window) // Verify if window was created
    {
        std::cout << "[ERROR]: Failed to create window" << std::endl;
        glfwTerminate(); // Finilize GLFW
        return -1;
    }

    glfwMakeContextCurrent(window);                                  // Creating a context
    gladLoadGL();                                                    // Loading GLAD
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback); // Create a viewport
    std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl;  // Gets OpenGL's version

    Texture textures[] // Textures data array (Plane)
    {
        Texture("D_Wood_01.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE), // Create a diffuse texture
        Texture("S_Wood_01.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE) // Create a specular texture
    };

    Shader shaderProgram("default.vert", "default.frag"); // Create a Shader Program
    std::vector<Vertex> modelVert(vertices, vertices + sizeof(vertices) / sizeof(Vertex));  // Process plane's vertices
    std::vector<GLuint>  modelInd(indices, indices + sizeof(indices) / sizeof(GLuint));     // Process plane's indices
    std::vector<Texture> modelTex(textures, textures + sizeof(textures) / sizeof(Texture)); // Process plane's textures

    Mesh floor(modelVert, modelInd, modelTex); // Create plane's model

    Shader lightShader("light.vert", "light.frag"); // Linking light's vertex and fragment shaders
    std::vector<Vertex> lightVert(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex)); // Process light's vertices
    std::vector<GLuint> lightInd(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));     // Process light's indices

    Mesh light(lightVert, lightInd, modelTex); // Create light's model

    glm::vec4 lightColor = glm::vec4(LIGHT_COLOR);     // Set light's color
    glm::vec3 lightPos = glm::vec3(LIGHT_POSITION);    // Set light's position
    glm::mat4 lightModel = glm::mat4(1.0f);            // Initialize light's matrix
    lightModel = glm::translate(lightModel, lightPos); // Assigns light's matrix's transformations

    glm::vec3 pyramidPos = glm::vec3(PLANE_POSITION);        // Set plane's position
    glm::mat4 pyramidModel = glm::mat4(1.0f);                // Initialize pyramid's matrix
    pyramidModel = glm::translate(pyramidModel, pyramidPos); // Assigns plane's matrix's transformations

    lightShader.Active(); // Actives light's shader program
    // Export important light's values to the GPU using uniforms
    glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
    glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);

    shaderProgram.Active(); // Actives pyramid's shader program
    // Export important planes's values to the GPU using uniforms
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
    glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

    Camera camera(WINDOW_WIDTH, WINDOW_HEIGHT, CAMERA_SENSITIVITY, CAMERA_SPEED, glm::vec3(0.0f, 0.4f, 1.5f)); // Create a camera

    glEnable(GL_DEPTH_TEST); // To avoid depth visual glitchs

    while (!glfwWindowShouldClose(window)) // Windows's loop
    {
        camera.Inputs(window);           // Gets all camera's inputs
        ProcessCloseWindowInput(window); // Process window's shutdown input

        glClearColor(WINDOW_BACKGROUND_COLOR);              // Set window's background's color
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear front buffer and depth buffer

        camera.UpdateMatrix(CAMERA_FOV, CAMERA_MIN_DISTANCE, CAMERA_MAX_DISTANCE); // Initialize camera's shaders

        floor.Draw(shaderProgram, camera);
        light.Draw(lightShader, camera);

        glfwSwapBuffers(window); // Swapp buffers
        glfwPollEvents();        // Process window's events
    }

// --- Delete what we don't need anymore ---------- //

    // Plane's model
    shaderProgram.Delete();

    // Light's model
    lightShader.Delete();

// ------------------------------------------------ //

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
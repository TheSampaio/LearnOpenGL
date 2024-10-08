// Standard includes
#include <array>

// OpenGL's loader must be included before GLFW
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <stb/stb_image.h>

// Project includes
#include "Debug.h"

int main()
{
    // Store window's title and size
    unsigned short width = 800, height = 600;
    const char* title = "Window | OpenGL";

    // Initializes GLFW and log it if failed
    if (!glfwInit())
    {
        Debug::Log("[ERRO] Failed to initialize GLFW.", true);
        return EXIT_FAILURE;
    }

    // Set-up OpenGL's version and compatibility mode
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Locks window's size
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    // Creates a window (Windowed mode)
    GLFWwindow* pWindow = glfwCreateWindow(width, height, title, nullptr, nullptr);

    // Verifies if the window was created
    if (!pWindow)
    {
        Debug::Log("[ERRO] Failed to create a window");
        return EXIT_FAILURE;
    }

    // Creates an OpenGL's context
    glfwMakeContextCurrent(pWindow);
    
    // Loads GLAD
    gladLoadGL();

    // Creates a viewport for the window
    glViewport(0, 0, width, height);

    // ===== Data Input ================================================================================================== //
    
    // Vertices's dynamic array
    const std::array<GLfloat, 32> vertices
    {
        // === Position        // === Color           // === UV
        -0.8f, -0.8f,  0.0f,    1.0f,  1.0f,  1.0f,    0.0f,  0.0f,
         0.8f, -0.8f,  0.0f,    1.0f,  1.0f,  1.0f,    1.0f,  0.0f,
        -0.8f,  0.8f,  0.0f,    1.0f,  1.0f,  1.0f,    0.0f,  1.0f,
         0.8f,  0.8f,  0.0f,    1.0f,  1.0f,  1.0f,    1.0f,  1.0f
    };

    // Indices's array
    const std::array<GLuint, 6> indices
    {
        0, 1, 2,
        2, 1, 3
    };

    // Vertex shader's source code (Temporary)
    const char* vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 inPosition;\n"
        "layout (location = 1) in vec3 inColor;\n"
        "layout (location = 3) in vec2 inUV;\n"
        "out vec4 outColor;\n"
        "out vec2 outUV;\n"
        "void main()\n"
        "{\n"
        "   outColor = vec4(inColor, 1.0);\n"
        "   outUV = inUV;\n"
        "   gl_Position = vec4(inPosition, 1.0);\n"
        "}\0";

    // Fragment shader's source code (Temporary)
    const char* fragmentShaderSource = "#version 330 core\n"
        "in vec4 outColor;\n"
        "in vec2 outUV;\n"
        "out vec4 gl_Color;\n"
        "uniform sampler2D DiffuseSampler;\n"
        "void main()\n"
        "{\n"
        "gl_Color = texture(DiffuseSampler, outUV) * outColor;\n"
        "}\0";

    // ===== Shaders and Shader Program ================================================================================== //

    // Creates identifies for the vertex shader, fragment shader and shader program
    GLuint vertexShader = 0, fragmentShader = 0, shaderProgram = 0;

    // Compiles vertex shader
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    // Compiles fragment shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    // Attaches both shaders to the shader program and links it
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Detaches both shaders to the shader program
    glDetachShader(shaderProgram, vertexShader);
    glDetachShader(shaderProgram, fragmentShader);

    // ===== Buffers Creation ============================================================================================ //
    
    // Creates identifies for the VAO (Vertex Array Object) and VBO (Vertex Buffer Object)
    GLuint VAO = 0, VBO = 0, EBO = 0;

    // Generates a VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Generates a VBO and set-ups it
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    // Generates a EBO and set-ups it
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    // Set-ups the VAO's layouts
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), nullptr);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<void*>(3 * sizeof(GLfloat)));
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<void*>(6 * sizeof(GLfloat)));

    // Enables the VAO's layouts
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(3);

    // Unbind VAO and VBO to avoid bugs
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // ===== Texture Creation ============================================================================================ //

    // Flips image on load
    stbi_set_flip_vertically_on_load(true);

    // Loads a image from disk
    int texWidth = 0, texHeight = 0, column = 0;
    unsigned char* bytes = stbi_load("../../Resources/Textures/meme-gigashad.png", &texWidth, &texHeight, &column, 0);

    // Generates a texture
    GLuint texture;
    glGenTextures(1, &texture);

    // Activates the texture slot 0
    glActiveTexture(GL_TEXTURE0);

    // Binds a 2D texture
    glBindTexture(GL_TEXTURE_2D, texture);

    // Set-ups the minification and magnification filters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Set-ups the texture's wrap behavior
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Set-ups the texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);

    // Generates a mipmap for the 2D texture
    glGenerateMipmap(GL_TEXTURE_2D);

    // Free the data loaded from disk
    stbi_image_free(bytes);

    // Unbinds the 2D texture to avoid bugs
    glBindTexture(GL_TEXTURE_2D, 0);

    // =================================================================================================================== //

    // Main loop (Game loop)
    while (!glfwWindowShouldClose(pWindow))
    {
        // Process all window's events
        glfwPollEvents();

        // Clears colour buffer and changes the window's background colour
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.2f, 0.2f, 0.4f, 1.0f);

        // Draw call scope (This scope is JUST to organize!)
        {
            // Informs OpenGL which shader program and VAO we want to use
            glUseProgram(shaderProgram);
            glBindVertexArray(VAO);

            // Creates a uniform sampler and binds the generated texture
            glUniform1i(glGetUniformLocation(shaderProgram, "uDiffuseSampler"), 0);
            glBindTexture(GL_TEXTURE_2D, texture);

            // Draw a triangle using the EBO set-up
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, nullptr);

            // Unbind everything binded to avoid bugs
            glBindTexture(GL_TEXTURE_2D, 0);
            glBindVertexArray(0);
            glUseProgram(0);
        }

        // Swaps window's buffers
        glfwSwapBuffers(pWindow);
    }

    // Deletes what we need anymore
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &EBO);
    glDeleteTextures(1, &texture);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteProgram(shaderProgram);

    // Destroying window's process
    glfwDestroyWindow(pWindow);
    
    // Finalizes GLFW
    glfwTerminate();
    return 0;
}

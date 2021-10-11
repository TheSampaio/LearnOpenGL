#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Vertex Shader source code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

//Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";

int main(void)
{
    unsigned int windowWidth = 800;                                         // Window's width
    unsigned int windowHeight = (windowWidth / 2) + (windowWidth / 2) / 2;  // Window's height
    const char* windowName = "My OpenGL Window";                            // Window's title

    glfwInit(); // Initializing GLFW

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                  // Setting OpenGL version to '3'.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);                  // Setting OpenGL version to 3.'3'
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // Choosing OpenGL's CORE mode

    if (!glfwInit()) // Verifying if GLFW was initialized
    {
        std::cout << "[ERROR]: Failed to initialize GLFW" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, windowName, NULL, NULL);  // Creating the window

    if (!window) // Verifying if window was initialized
    {
        std::cout << "[ERROR]: Failed to create a window" << std::endl;
        glfwTerminate(); // Finilizing GLFW
        return -1;
    }

    glfwMakeContextCurrent(window); // Creating a context
    gladLoadGL();                   // Loading GLAD

    glViewport(0, 0, windowWidth, windowHeight); // Creating a viewport

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);     // Creating a vertex shader
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // Linking the vertexShaderSource to the vertex shader 
    glCompileShader(vertexShader);                              // Compiling vertex shader into machine code

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);     // Creating a fragment shader
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL); // Linking the fragmentShaderSource to the fragment shader
    glCompileShader(fragmentShader);                                // Compiling fragment shader into machine code

    GLuint shaderProgram = glCreateProgram();       // Creating a shader program
    glAttachShader(shaderProgram, vertexShader);    // Linking the vertex shader to the shader program
    glAttachShader(shaderProgram, fragmentShader);  // Linking the fragment shader to the shader program
    glLinkProgram(shaderProgram);                   // Mounting the shader program with vertex and fragment shaders

    glDeleteShader(vertexShader);   // Delleting vertex shader after link
    glDeleteShader(fragmentShader); // Delleting fragment shader after link

    // Array vertices data
    GLfloat vertices[]
    {
        -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,    // Left corner
         0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,    // Right corner
         0.0f,  0.5f * float(sqrt(3)) * 2 / 3, 0.0f // Top corner
    };

    GLuint VAO, VBO; // Defining a Vertex Array Object & Vertex Buffer Object

    glGenVertexArrays(1, &VAO); // Creating the VAO
    glGenBuffers(1, &VBO); // Creating the VBO

    glBindVertexArray(VAO);             // Bind the VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // Bind the VBO

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Storing the vertices in the VBO

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // Configuring the VAO
    glEnableVertexAttribArray(0);                                                 // Enable the VAO to can use it

    glBindBuffer(GL_ARRAY_BUFFER, 0);   // To avoid bugs
    glBindVertexArray(0);               // To avoid bugs

    while (!glfwWindowShouldClose(window)) // Windows's loop
    {
        glClearColor(0.08f, 0.14f, 0.18f, 1.0f); // Choosing window's background's color
        glClear(GL_COLOR_BUFFER_BIT);            // Cleaning up front buffer

        glUseProgram(shaderProgram); // Active the shader program

        glBindVertexArray(VAO);             // Bind the VAO again
        glDrawArrays(GL_TRIANGLES, 0, 3);   // Tell OpenGL what to draw

        glfwSwapBuffers(window); // Swapping buffers
        glfwPollEvents();        // Processing window's events
    }

    // Deleting what we don't need anymore
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);  // Destroying window's process
    glfwTerminate();            // Finilizing GLFW
    return 0;
}
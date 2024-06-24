#include "PCH.h"
#include "Skybox.h"

#include "Camera.h"
#include "Renderer.h"
#include "Window.h"

static GLfloat skyboxVertices[] =
{
    //   Coordinates
    -1.0f, -1.0f,  1.0f, //        7--------6
     1.0f, -1.0f,  1.0f, //       /|       /|
     1.0f, -1.0f, -1.0f, //      4--------5 |
    -1.0f, -1.0f, -1.0f, //      | |      | |
    -1.0f,  1.0f,  1.0f, //      | 3------|-2
     1.0f,  1.0f,  1.0f, //      |/       |/
     1.0f,  1.0f, -1.0f, //      0--------1
    -1.0f,  1.0f, -1.0f
};

static GLuint skyboxIndices[] =
{
    // Right
    1, 2, 6,
    6, 5, 1,

    // Left
    0, 4, 7,
    7, 3, 0,

    // Top
    4, 5, 6,
    6, 7, 4,

    // Bottom
    0, 3, 2,
    2, 1, 0,

    // Back
    0, 1, 5,
    5, 4, 0,

    // Front
    3, 7, 6,
    6, 2, 3
};

Skybox::Skybox(std::array<std::string, 6>& textures)
{
    // Create VAO, VBO, and EBO for the skybox
    glGenVertexArrays(1, &m_VertexArray);
    glGenBuffers(1, &m_VertexBuffer);
    glGenBuffers(1, &m_ElementBuffer);

    glBindVertexArray(m_VertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ElementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Creates the cubemap texture object
    glGenTextures(1, &m_Texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_Texture);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // These are very important to prevent seams
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    // Cycles through all the textures and attaches them to the cubemap object
    for (unsigned int i = 0; i < 6; i++)
    {
        int width, height, nrChannels;
        unsigned char* data = stbi_load(textures[i].c_str(), &width, &height, &nrChannels, 0);

        if (data)
        {
            stbi_set_flip_vertically_on_load(false);
            glTexImage2D
            (
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0,
                GL_RGB,
                width,
                height,
                0,
                GL_RGB,
                GL_UNSIGNED_BYTE,
                data
            );

            stbi_image_free(data);
        }

        else
        {
            std::cout << "Failed to load texture: " << textures[i] << std::endl;
            stbi_image_free(data);
        }
    }
}

Skybox::~Skybox()
{
    glDeleteTextures(1, &m_Texture);
    glDeleteBuffers(1, &m_ElementBuffer);
    glDeleteBuffers(1, &m_VertexBuffer);
    glDeleteVertexArrays(1, &m_VertexArray);
}

void Skybox::Draw(Shader& shader, Camera& camera)
{
    Renderer& renderer = Renderer::GetInstance();
    Window& window = Window::GetInstance();

    glDepthFunc(GL_LEQUAL);

    shader.Bind();
    glm::mat4 view = glm::mat4{ 1.0f };
    glm::mat4 projection = glm::mat4{ 1.0f };

    view = glm::mat4(glm::mat3(glm::lookAt(camera.GetPosition(), camera.GetPosition() + camera.GetRotation(), camera.GetVectorUp())));
    projection = glm::perspective(glm::radians(camera.GetFieldOfView()),
        static_cast<float>(window.GetSize()[0]) / static_cast<float>(window.GetSize()[1]),
        camera.GetClipDistance()[0], camera.GetClipDistance()[1]);

    renderer.SetUniform1i(shader, "uSkybox", 0);
    renderer.SetUniformMatrix4fv(shader, "uView", view);
    renderer.SetUniformMatrix4fv(shader, "uProjection", projection);

    // Draw skybox
    glBindVertexArray(m_VertexArray);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_Texture);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    shader.Unbind();

    // Switch back to the normal depth function
    glDepthFunc(GL_LESS);
}

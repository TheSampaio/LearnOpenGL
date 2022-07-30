#include "Framebuffer.h"

Framebuffer::~Framebuffer()
{
    Delete();
}

Framebuffer::Framebuffer(Camera& camera)
{
    // Prepare framebuffer rectangle VBO and VAO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), &Vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    // Create FrameBuffer Object
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    // Create Framebuffer Texture
    glGenTextures(1, &Texture);
    glBindTexture(GL_TEXTURE_2D, Texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, camera.width, camera.height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Prevents edge bleeding

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Texture, 0);

    // Create Render Buffer Object
    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, camera.width, camera.height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

    // Framebuffer and Renderbuffer status
    GLenum renderBufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (renderBufferStatus != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "[ERRO] Framebuffer error: " << renderBufferStatus << std::endl;
}

void Framebuffer::Start(Camera& camera, glm::vec3 backgroundcolor)
{
    // Fix framebuffer's aspect ratio
    glBindTexture(GL_TEXTURE_2D, Texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, camera.width, camera.height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    // Fix renderbuffer's aspect ratio and use it
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, camera.width, camera.height);

    glClearColor(backgroundcolor.r, backgroundcolor.g, backgroundcolor.b, 1.0f); // Background color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);  // Clear front buffer and depth buffer

    glEnable(GL_DEPTH_TEST);
}

void Framebuffer::Finish(Shader& shader)
{
    // Bind and draw framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    shader.Active();
    glBindVertexArray(VAO);

    glDisable(GL_DEPTH_TEST);
    glBindTexture(GL_TEXTURE_2D, Texture);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Framebuffer::Delete()
{
    // Delete everything that we don't need
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteFramebuffers(1, &FBO);
    glDeleteRenderbuffers(1, &RBO);
}

#include "PCH.h"
#include "BufferFrame.h"

#include "Renderer.h"
#include "Window.h"

static GLfloat vertices[] =
{
    // Coords    // texCoords
     1.0f, -1.0f,  1.0f, 0.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
    -1.0f,  1.0f,  0.0f, 1.0f,

     1.0f,  1.0f,  1.0f, 1.0f,
     1.0f, -1.0f,  1.0f, 0.0f,
    -1.0f,  1.0f,  0.0f, 1.0f
};

BufferFrame::BufferFrame()
{
    // Creates the screen's rectangle
    glGenVertexArrays(1, &m_VertexArray);
    glGenBuffers(1, &m_VertexBuffer);

    glBindVertexArray(m_VertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    // Creates the framebuffer
    glGenFramebuffers(1, &m_Framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);

    // Creates the framebuffer's texture
    glGenTextures(1, &m_Texture);
    glBindTexture(GL_TEXTURE_2D, m_Texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, Window::GetInstance().GetSize()[0], Window::GetInstance().GetSize()[1], 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Prevents edge bleeding
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Texture, 0);

    // Creates the renderbuffer
    glGenRenderbuffers(1, &m_Renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_Renderbuffer);

    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Window::GetInstance().GetSize()[0], Window::GetInstance().GetSize()[1]);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_Renderbuffer);

    // Error checking
    auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer error: " << fboStatus << std::endl;
}

BufferFrame::~BufferFrame()
{
    glDeleteBuffers(1, &m_VertexBuffer);
    glDeleteVertexArrays(1, &m_VertexArray);

    glDeleteTextures(1, &m_Texture);
    glDeleteRenderbuffers(1, &m_Renderbuffer);
    glDeleteFramebuffers(1, &m_Framebuffer);
}

void BufferFrame::Use(Shader& shader)
{
    // Draw the framebuffer rectangle
    shader.Bind();
    glBindVertexArray(m_VertexArray);

    // Send data to GPU
    Renderer::GetInstance().SetUniform1i(shader, "uSurface", 0);

    // Prevents framebuffer rectangle from being discarded
    glDisable(GL_DEPTH_TEST);

    // Activate texture's slot 0 and draw the frame buffer with a texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_Texture);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

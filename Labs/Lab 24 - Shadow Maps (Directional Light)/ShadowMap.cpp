#include "PCH.h"
#include "ShadowMap.h"

#include "Renderer.h"
#include "Window.h"

ShadowMap::ShadowMap(glm::vec3 direction, float area, float distance, unsigned short resolution)
{
    m_Size = { resolution, resolution };

    // Framebuffer for Shadow Map
    glGenFramebuffers(1, &m_Framebuffer);

    // Texture for Shadow Map FBO
    glGenTextures(1, &m_Texture);
    glBindTexture(GL_TEXTURE_2D, m_Texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_Size[0], m_Size[1], 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    // Prevents darkness outside the frustrum
    float clampColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, clampColor);

    glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_Texture, 0);

    // Needed since we don't touch the color buffer
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Matrices needed for the light's perspective
    m_Orthographic = glm::ortho(-area, area, -area, area, 0.2f, distance);
    m_View = glm::lookAt(20.0f * direction, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    m_Projection = m_Orthographic * m_View;
}

ShadowMap::~ShadowMap()
{
    glDeleteTextures(1, &m_Texture);
    glDeleteFramebuffers(1, &m_Framebuffer);
}

void ShadowMap::Begin(Shader& shader)
{
    shader.Bind();
    Renderer::GetInstance().SetUniformMatrix4fv(shader, "uLightProjection", m_Projection);

    // Preparations for the Shadow Map
    glViewport(0, 0, m_Size[0], m_Size[1]);
    glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);
    glClear(GL_DEPTH_BUFFER_BIT);

    // Draw scene for shadow map
    glCullFace(GL_BACK);
}

void ShadowMap::Draw(Shader& shader)
{
    Renderer& renderer = Renderer::GetInstance();

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_Texture);

    renderer.SetUniform1i(shader, "uShadowMap", 2);
    renderer.SetUniformMatrix4fv(shader, "uLightProjection", m_Projection);
}

void ShadowMap::End()
{
    Window& window = Window::GetInstance();

    glCullFace(GL_FRONT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, window.GetSize()[0], window.GetSize()[1]);
}

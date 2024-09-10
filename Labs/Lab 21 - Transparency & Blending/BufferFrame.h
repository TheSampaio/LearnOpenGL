#ifndef LEARNOPENGL_FRAMEBUFFER_H
#define LEARNOPENGL_FRAMEBUFFER_H

class Shader;

class BufferFrame
{
public:
	BufferFrame();
	~BufferFrame();

	void Bind() { glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer); }
	void Unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

	void Use(Shader& shader);

private:
	GLuint m_VertexArray, m_VertexBuffer, m_Framebuffer, m_Texture, m_Renderbuffer;
};

#endif // !LEARNOPENGL_FRAMEBUFFER_H

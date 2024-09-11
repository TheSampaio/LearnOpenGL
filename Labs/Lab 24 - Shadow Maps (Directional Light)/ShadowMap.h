#ifndef LEARNOPENGL_SHADOWMAP_H
#define LEARNOPENGL_SHADOWMAP_H

class Shader;

class ShadowMap
{
public:
	ShadowMap(glm::vec3 direction, float area = 50.0f, float distance = 60.0f, unsigned short resolution = 2048);
	~ShadowMap();

	glm::mat4 GetProjectionMatrix() const { return m_Projection; }

	void Begin(Shader& shader);
	void Draw(Shader& shader);
	void End();

private:
	GLuint m_Framebuffer, m_Texture;
	std::array<unsigned short, 2> m_Size;
	glm::mat4 m_Orthographic, m_View, m_Projection;
};

#endif // !LEARNOPENGL_SHADOWMAP_H

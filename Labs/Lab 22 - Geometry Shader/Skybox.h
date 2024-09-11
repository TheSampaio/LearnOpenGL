#ifndef LEARNOPENGL_SKYBOX_H
#define LEARNOPENGL_SKYBOX_H

class Camera;
class Shader;

class Skybox
{
public:
	Skybox(std::array<std::string, 6>& textures);
	~Skybox();

	void Draw(Shader& shader, Camera& camera);

private:
	GLuint m_VertexArray, m_VertexBuffer, m_ElementBuffer, m_Texture;
};

#endif // !LEARNOPENGL_SKYBOX_H

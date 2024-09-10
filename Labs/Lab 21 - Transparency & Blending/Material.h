#ifndef LEARNOPENGL_MATERIAL_H
#define LEARNOPENGL_MATERIAL_H

class Shader;
class Texture;

class Material
{
public:
	Material(Texture* diffuse, Texture* specular, GLfloat Intensity = 1.0f, GLfloat shininess = 1.0f);

	void Bind(Shader& shader);
	void Unbind();

private:
	Texture* m_Diffuse;
	Texture* m_Specular;
	GLfloat m_Intensity, m_Shininess;
};

#endif // !LEARNOPENGL_MATERIAL_H

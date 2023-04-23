#ifndef LEARNOPENGL_TEXTURE_H
#define LEARNOPENGL_TEXTURE_H

class Texture
{
public:
	Texture(const char* FilePath, GLenum Slot = GL_TEXTURE0, GLenum Format = GL_RGBA, GLenum Filter = GL_LINEAR);
	~Texture();

	// Sets the texture's sampler uniform
	void SetUniform(class Shader& ShaderProgram, const char* Uniform, GLuint Unit);

	// Binds the texture
	inline void Bind() { glBindTexture(GL_TEXTURE_2D, m_Id); }

	// Unbinds the texture
	inline void Unbind() { glBindTexture(GL_TEXTURE_2D, 0); }

	// Returns the texture's ID
	inline GLuint GetId() { return m_Id; }

	// Returns the texture's size
	inline std::array<int, 2> GetSize() { return m_Size; }

private:
	// Attributes
	GLuint m_Id;
	std::array<int, 2> m_Size;
	int m_Column;
};

#endif // !LEARNOPENGL_TEXTURE_H

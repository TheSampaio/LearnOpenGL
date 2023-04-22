#ifndef LEARNOPENGL_TEXTURE_H
#define LEARNOPENGL_TEXTURE_H

class Texture
{
public:
	Texture(const char* FilePath, GLenum Slot = GL_TEXTURE0, GLenum Format = GL_RGBA, GLenum Filter = GL_LINEAR);
	~Texture();

	// === Main methods ===

	void SetUniform(class Shader& ShaderProgram, const char* Uniform, GLuint Unit);

	inline void Bind()   { glBindTexture(GL_TEXTURE_2D, m_Id); }
	inline void Unbind() { glBindTexture(GL_TEXTURE_2D, 0); }

	// === Get methods ===

	inline GLuint GetId()               { return m_Id; }
	inline std::array<int, 2> GetSize() { return m_Size; }

private:
	GLuint m_Id;
	std::array<int, 2> m_Size;
	int m_Column;
};

#endif // !LEARNOPENGL_TEXTURE_H

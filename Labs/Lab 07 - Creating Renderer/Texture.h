#ifndef LEARNOPENGL_TEXTURE_H
#define LEARNOPENGL_TEXTURE_H

class Texture
{
public:
	Texture(const char* FilePath, GLenum Slot = GL_TEXTURE0, GLenum Format = GL_RGBA, GLenum Filter = GL_LINEAR);
	~Texture();

	// Binds the texture
	inline void Bind() { glBindTexture(GL_TEXTURE_2D, m_Id); }

	// Unbinds the texture
	inline void Unbind() { glBindTexture(GL_TEXTURE_2D, 0); }

	// Returns the texture's ID
	inline GLuint GetId() const { return m_Id; }

	// Returns the texture's size
	inline std::array<int, 2> GetSize() const { return m_Size; }

private:
	// Attributes
	GLuint m_Id;
	std::array<int, 2> m_Size;
	int m_Column;
};

#endif // !LEARNOPENGL_TEXTURE_H

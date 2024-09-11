#ifndef LEARNOPENGL_SHADER_H
#define LEARNOPENGL_SHADER_H

class Shader
{
public:
	Shader(const char* vertexShaderFile, const char* fragmentShaderFile, const char* geometryShader = nullptr);
	~Shader();

	// Binds the shader program
	inline void Bind() { glUseProgram(m_Id); }

	// Unbinds the shader program
	inline void Unbind() { glUseProgram(0); }

	// Returns the shader program's ID
	inline GLuint& GetId() { return m_Id; }

private:
	// Attributes
	GLuint m_Id,
		m_VertexShader,
		m_FragmentShader,
		m_GeometryShader;

	std::string LoadShader(const char* filePath);
	void CompileShader(const char* filePath, GLuint& shaderId, GLenum shaderType);
	void CreateShaderProgram(GLuint vertexShader, GLuint fragmentShader);
	void CreateShaderProgram(GLuint vertexShader, GLuint fragmentShader, GLuint geometryShader);
};

#endif // !LEARNOPENGL_SHADER_H

#include "PCH.h"
#include "Shader.h"

#include "Debug.h"

Shader::Shader(const char* VertexShaderFile, const char* FragmentShaderFile)
{
	// Compiles shaders
	CompileShader(VertexShaderFile, m_VertexShader, GL_VERTEX_SHADER);
	CompileShader(FragmentShaderFile, m_FragmentShader, GL_FRAGMENT_SHADER);

	// Creates and links the shader program
	CreateShaderProgram(m_VertexShader, m_FragmentShader);
}

Shader::~Shader()
{
	// Deletes what we need anymore
	glDeleteShader(m_VertexShader);
	glDeleteShader(m_FragmentShader);
	glDeleteProgram(m_Id);
}

std::string Shader::LoadShader(const char* FilePath)
{
	std::fstream file;
	std::string source;
	std::string content;

	// Opens and reads a text file
	file.open(FilePath, std::ios::in);

	if (file.is_open())
	{
		while (std::getline(file, source))
		{
			content.append(source + "\n");
		}

		file.close();
	}

	// Logs an error if the text file was empty
	if (content.empty())
	{
		Debug::Log("[ERRO] Failed to read shaders.");
	}

	return content;
}

void Shader::CompileShader(const char* FilePath, GLuint& ShaderId, GLenum ShaderType)
{
	std::string source = LoadShader(FilePath);
	const char* pSource = source.c_str();

	ShaderId = glCreateShader(ShaderType);
	glShaderSource(ShaderId, 1, &pSource, nullptr);
	glCompileShader(ShaderId);

	// The shader id must be a compiled shader
	GLint result = GL_TRUE;
	glGetShaderiv(ShaderId, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE)
	{
		// Get log's length 
		GLint infoLogLength = 0;
		glGetShaderiv(ShaderId, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0)
		{
			std::string shaderInfoLog(infoLogLength, '\0');
			glGetShaderInfoLog(ShaderId, infoLogLength, nullptr, &shaderInfoLog[0]);

			std::printf("[ERRO] Failed to compile shader \"%s\"", shaderInfoLog.c_str());
			char Stop = std::getchar();
		}
	}
}

void Shader::CreateShaderProgram(GLuint VextexShader, GLuint FragmentShader)
{
	m_Id = glCreateProgram();
	glAttachShader(m_Id, VextexShader);
	glAttachShader(m_Id, FragmentShader);
	glLinkProgram(m_Id);

	// Check if the link was successful
	GLint result = GL_TRUE;
	glGetProgramiv(m_Id, GL_LINK_STATUS, &result);

	if (result == GL_FALSE)
	{
		GLint infoLogLength = 0;
		glGetProgramiv(m_Id, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0)
		{
			std::string programInfoLog(infoLogLength, '\0');
			glGetProgramInfoLog(m_Id, infoLogLength, nullptr, &programInfoLog[0]);

			std::printf("[ERROR] Failed to link shader program \"%s\"", programInfoLog.c_str());
			char Stop = std::getchar();
		}
	}

	// Detach what weshaders from program
	glDetachShader(m_Id, m_VertexShader);
	glDetachShader(m_Id, m_FragmentShader);
}
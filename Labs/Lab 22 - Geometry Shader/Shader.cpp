#include "PCH.h"
#include "Shader.h"

#include "Debug.h"

Shader::Shader(const char* vertexShaderFile, const char* fragmentShaderFile, const char* geometryShader)
{
	// Compiles shaders
	CompileShader(vertexShaderFile, m_VertexShader, GL_VERTEX_SHADER);
	CompileShader(fragmentShaderFile, m_FragmentShader, GL_FRAGMENT_SHADER);

	// Creates and links the shader program
	if (!geometryShader)
		CreateShaderProgram(m_VertexShader, m_FragmentShader);

	else
		CompileShader(geometryShader, m_GeometryShader, GL_GEOMETRY_SHADER);
		CreateShaderProgram(m_VertexShader, m_FragmentShader, m_GeometryShader);
}

Shader::~Shader()
{
	// Deletes what we need anymore
	glDeleteShader(m_VertexShader);
	glDeleteShader(m_FragmentShader);

	if (m_GeometryShader)
		glDeleteShader(m_GeometryShader);

	glDeleteProgram(m_Id);
}

std::string Shader::LoadShader(const char* filePath)
{
	std::fstream file;
	std::string source;
	std::string content;

	// Opens and reads a text file
	file.open(filePath, std::ios::in);

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

void Shader::CompileShader(const char* filePath, GLuint& shaderId, GLenum shaderType)
{
	std::string source = LoadShader(filePath);
	const char* pSource = source.c_str();

	shaderId = glCreateShader(shaderType);
	glShaderSource(shaderId, 1, &pSource, nullptr);
	glCompileShader(shaderId);

	// The shader id must be a compiled shader
	GLint result = GL_TRUE;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE)
	{
		// Get log's length 
		GLint infoLogLength = 0;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 0)
		{
			std::string shaderInfoLog(infoLogLength, '\0');
			glGetShaderInfoLog(shaderId, infoLogLength, nullptr, &shaderInfoLog[0]);

			std::printf("[ERRO] Failed to compile shader \"%s\"", shaderInfoLog.c_str());
			char stop = std::getchar();
		}
	}
}

void Shader::CreateShaderProgram(GLuint vertexShader, GLuint fragmentShader)
{
	m_Id = glCreateProgram();
	glAttachShader(m_Id, vertexShader);
	glAttachShader(m_Id, fragmentShader);
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
			char stop = std::getchar();
		}
	}

	// Detach what weshaders from program
	glDetachShader(m_Id, m_VertexShader);
	glDetachShader(m_Id, m_FragmentShader);
}

void Shader::CreateShaderProgram(GLuint vertexShader, GLuint fragmentShader, GLuint geometryShader)
{
	m_Id = glCreateProgram();
	glAttachShader(m_Id, vertexShader);
	glAttachShader(m_Id, fragmentShader);
	glAttachShader(m_Id, geometryShader);
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
			char stop = std::getchar();
		}
	}

	// Detach what weshaders from program
	glDetachShader(m_Id, m_VertexShader);
	glDetachShader(m_Id, m_FragmentShader);
	glDetachShader(m_Id, m_GeometryShader);
}
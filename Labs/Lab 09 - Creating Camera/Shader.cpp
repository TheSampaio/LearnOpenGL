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
	std::fstream File;
	std::string Source;
	std::string Content;

	// Opens and reads a text file
	File.open(FilePath, std::ios::in);

	if (File.is_open())
	{
		while (std::getline(File, Source))
		{
			Content.append(Source + "\n");
		}

		File.close();
	}

	// Logs an error if the text file was empty
	if (Content.empty())
	{
		Debug::Log("[ERRO] Failed to read shaders.");
	}

	return Content;
}

void Shader::CompileShader(const char* FilePath, GLuint& ShaderId, GLenum ShaderType)
{
	std::string Source = LoadShader(FilePath);
	const char* pSource = Source.c_str();

	ShaderId = glCreateShader(ShaderType);
	glShaderSource(ShaderId, 1, &pSource, nullptr);
	glCompileShader(ShaderId);

	// The shader id must be a compiled shader
	GLint Result = GL_TRUE;
	glGetShaderiv(ShaderId, GL_COMPILE_STATUS, &Result);

	if (Result == GL_FALSE)
	{
		// Get log's length 
		GLint InfoLogLength = 0;
		glGetShaderiv(ShaderId, GL_INFO_LOG_LENGTH, &InfoLogLength);

		if (InfoLogLength > 0)
		{
			std::string ShaderInfoLog(InfoLogLength, '\0');
			glGetShaderInfoLog(ShaderId, InfoLogLength, nullptr, &ShaderInfoLog[0]);

			std::printf("[ERRO] Failed to compile shader \"%s\"", ShaderInfoLog.c_str());
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
	GLint Result = GL_TRUE;
	glGetProgramiv(m_Id, GL_LINK_STATUS, &Result);

	if (Result == GL_FALSE)
	{
		GLint InfoLogLength = 0;
		glGetProgramiv(m_Id, GL_INFO_LOG_LENGTH, &InfoLogLength);

		if (InfoLogLength > 0)
		{
			std::string ProgramInfoLog(InfoLogLength, '\0');
			glGetProgramInfoLog(m_Id, InfoLogLength, nullptr, &ProgramInfoLog[0]);

			std::printf("[ERROR] Failed to link shader program \"%s\"", ProgramInfoLog.c_str());
			char Stop = std::getchar();
		}
	}

	// Detach what weshaders from program
	glDetachShader(m_Id, m_VertexShader);
	glDetachShader(m_Id, m_FragmentShader);
}
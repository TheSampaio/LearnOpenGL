#include"shaderClass.h"

// Reads a text file and outputs a string with everything in the text file
std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

// Constructor that build the Shader Program from 2 different shaders
Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
	// Read vertexFile and fragmentFile and store the strings
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	// Convert the shader source strings into character arrays
	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();
	
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER); // Create Vertex Shader Object and get its reference
	glShaderSource(vertexShader, 1, &vertexSource, NULL);	// Attach Vertex Shader source to the Vertex Shader Object
	glCompileShader(vertexShader);							// Compile the Vertex Shader into machine code
	CompileErrors(vertexShader, "VERTEX");					// Error log if the Vertex Shader don't works
	
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // Create Fragment Shader Object and get its reference
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);	// Attach Fragment Shader source to the Fragment Shader Object
	glCompileShader(fragmentShader);							// Compile the Vertex Shader into machine code
	CompileErrors(vertexShader, "FRAGMENT");					// Error log if the Fragment Shader don't works
		
	ID = glCreateProgram(); // Create Shader Program Object and get its reference
	
	glAttachShader(ID, vertexShader);	// Attach the Vertex Shader to the Shader Program
	glAttachShader(ID, fragmentShader); // Attach the Fragment Shader to the Shader Program
	
	glLinkProgram(ID);				// Link all the shaders together into the Shader Program
	CompileErrors(ID, "PROGRAM");	// Error log if the Shader Program don't works

	// Delete the now useless Vertex and Fragment Shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::Active() // Actives the Shader Program
{
	glUseProgram(ID);
}

void Shader::Delete() // Deletes the Shader Program
{
	glDeleteProgram(ID);
}

void Shader::CompileErrors(unsigned int shader, const char* type)
{
	GLint hasCompiled;
	char infoLog[500];

	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);

		if (hasCompiled == GL_FALSE)
		{
			glGetShaderInfoLog(shader, 500, NULL, infoLog);
			std::cout << "[SHADER COMPILATION ERROR]: " << type << std::endl << std::endl;
		}
	}

	else 
	{
		glGetProgramiv(shader, GL_COMPILE_STATUS, &hasCompiled);

		if (hasCompiled == GL_FALSE)
		{
			glGetProgramInfoLog(shader, 500, NULL, infoLog);
			std::cout << "[SHADER LINKING ERROR]: " << type << std::endl << std::endl;
		}
	}
}
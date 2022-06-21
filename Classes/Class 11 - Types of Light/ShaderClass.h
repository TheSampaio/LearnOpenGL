#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include<glad/glad.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>

std::string get_file_contents(const char* filename);

class Shader
{
public:
	
	GLuint ID; // Reference ID of the Shader Program
	Shader(const char* vertexFile, const char* fragmentFile); // Constructor that build the Shader Program from 2 different shaders

	void Active();	// Activates the Shader Program
	void Delete();	// Deletes the Shader Program

private:
	void CompileErrors(unsigned int shader, const char* type); // Function that verify shader's compilation's errors
};
#endif
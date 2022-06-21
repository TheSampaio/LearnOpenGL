#version 330 core // Definig the shader's version

in vec3 color;		// Importing the color variable from vertex shader
out vec4 FragColor; // Definig a variable to store colors data to export it

void main()
{
   FragColor = vec4(color, 1.0f); // Applying shader's color
}
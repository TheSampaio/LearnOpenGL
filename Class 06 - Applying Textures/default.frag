#version 330 core // Definig GLSL's version

in vec3 color;	   // Importing color variable from vertex shader
in vec2 texCoord;  // Importing texture's coordinates from vertex shader

out vec4 FragColor; // Definig a variable to store colors data to export it

uniform sampler2D tex0;  // Defining a uniform variable to use texture's coordinates from the buffer data

void main()
{
   FragColor = texture(tex0, texCoord); // Exporting the imported texture to show it on screen
}
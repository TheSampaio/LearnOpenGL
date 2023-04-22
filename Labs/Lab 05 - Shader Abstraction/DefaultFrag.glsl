// Sets the GLSL's version and compatibility mode
#version 330 core

// Input variables
in vec4 outColour;

// Output variables
out vec4 gl_Color;

void main()
{
    // Final fragment colour
    gl_Color = outColour;
};

// Sets the GLSL's version and compatibility mode
#version 330 core

// Set-ups GLSL's layouts (Input variables)
layout (location = 0) in vec3 iPosition;
layout (location = 1) in vec3 iColour;
layout (location = 3) in vec2 iUV;

// Output variables
out vec4 vColour;
out vec2 vUV;

void main()
{
    // Send data to the fragment shader
    vColour = vec4(iColour, 1.0);
    vUV = iUV;

    // Final vertex position
    gl_Position = vec4(iPosition, 1.0);
}

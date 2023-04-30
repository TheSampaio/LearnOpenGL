// Sets the GLSL's version and compatibility mode
#version 330 core

// Set-ups GLSL's layouts (Input variables)
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inColour;
layout (location = 3) in vec2 inUV;

// Output variables
out vec4 outColour;
out vec2 outUV;

// Uniforms
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main()
{
    // Send data to the fragment shader
    outColour = vec4(inColour, 1.0);
    outUV = inUV;

    // Final vertex position
    gl_Position = Projection * View * Model * vec4(inPosition, 1.0);
};

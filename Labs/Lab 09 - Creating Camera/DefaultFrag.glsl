// Sets the GLSL's version and compatibility mode
#version 330 core

// Input variables
in vec4 outColour;
in vec2 outUV;

// Output variables
out vec4 gl_Color;

// Uniforms
uniform sampler2D DiffuseSampler;

void main()
{
    // Final fragment colour
    gl_Color = texture(DiffuseSampler, outUV) * outColour;
};

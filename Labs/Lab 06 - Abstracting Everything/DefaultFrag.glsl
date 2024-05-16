// Sets the GLSL's version and compatibility mode
#version 330 core

// Input variables
in vec4 vColour;
in vec2 vUV;

// Output variables
out vec4 oFragment;

// Uniforms
uniform sampler2D uDiffuseSampler;

void main()
{
    // Final fragment colour
    oFragment = texture(uDiffuseSampler, vUV) * vColour;
};

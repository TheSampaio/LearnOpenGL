#version 330 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inColour;

out vec4 outColour;

void main()
{
    outColour = vec4(inColour, 1.0);
    gl_Position = vec4(inPosition, 1.0);
};

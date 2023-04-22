#version 330 core

in vec4 outColour;
out vec4 gl_Color;

void main()
{
    gl_Color = outColour;
};

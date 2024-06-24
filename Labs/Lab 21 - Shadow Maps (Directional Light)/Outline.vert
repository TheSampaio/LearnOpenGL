#version 330 core

// Input
layout (location = 0) in vec3 iPosition;

// Uniforms

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform float uOutline;

void main()
{
	// Calculates th current fragment position
    vec3 fragmentPosition = vec3(uModel * uOutline * vec4(iPosition, 1.0));

	// Final vertex position
    gl_Position = uProjection * uView * vec4(fragmentPosition, 1.0);
}

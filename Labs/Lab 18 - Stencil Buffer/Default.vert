// Sets the GLSL's version and compatibility mode
#version 330 core

// Set-ups GLSL's layouts (Input variables)
layout (location = 0) in vec3 iPosition;
layout (location = 1) in vec3 iColour;
layout (location = 2) in vec3 iNormal;
layout (location = 3) in vec2 iTex;

// Output variables
out vec4 vColour;
out vec3 vNormal;
out vec2 vTex;
out vec3 vFragmentPosition;

// Uniforms
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform mat4 uNormalMatrix;

void main()
{
    // Send data to the fragment shader
    vColour = vec4(iColour, 1.0);

    // Transform normal from object space to world space
    vNormal = mat3(uNormalMatrix) * iNormal;
    vTex = iTex;

    // Calculates th current fragment position
    vFragmentPosition = vec3(uModel * vec4(iPosition, 1.0));

    // Final vertex position
    gl_Position = uProjection * uView * vec4(vFragmentPosition, 1.0);
}

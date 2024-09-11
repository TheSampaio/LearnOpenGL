// Sets the GLSL's version and compatibility mode
#version 330 core

// Set-ups GLSL's layouts (Input variables)
layout (location = 0) in vec3 iPosition;
layout (location = 1) in vec3 iColour;
layout (location = 2) in vec3 iNormal;
layout (location = 3) in vec2 iTex;

out DATA
{
	vec4 Colour;
	vec3 Normal;
	vec2 Tex;
	vec3 FragmentPosition;
    vec4 FragmentPositionLight;
    mat4 CameraMatrix;

} data_out;

// Uniforms
uniform float uScale;
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform mat4 uNormalMatrix;
uniform mat4 uLightProjection;

void main()
{
    // Send data to the fragment shader
    data_out.Colour = vec4(iColour, 1.0);

    // Transform normal from object space to world space
    data_out.Normal = mat3(uNormalMatrix) * iNormal;
    data_out.Tex = iTex * uScale;

    // Calculates the current fragment position for the model
    data_out.FragmentPosition = vec3(uModel * vec4(iPosition, 1.0));

    // Calculates the current fragment position for the light
    data_out.FragmentPositionLight = uLightProjection * vec4(data_out.FragmentPosition, 1.0);

    // Final vertex position
    data_out.CameraMatrix = uProjection * uView;
    gl_Position = vec4(data_out.FragmentPosition, 1.0);
}

// Sets the GLSL's version and compatibility mode
#version 330 core

#define INTENSITY 0.06
#define SPEED 1.0

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

// Output variables
out vec4 vColour;
out vec3 vNormal;
out vec2 vTex;
out vec3 vFragmentPosition;
out vec4 vFragmentPositionLight;
out mat4 vCameraMatrix;

in DATA
{
	vec4 Colour;
	vec3 Normal;
	vec2 Tex;
    vec3 FragmentPosition;
    vec4 FragmentPositionLight;
    mat4 CameraMatrix;

} data_in[];

uniform float uTime;
uniform vec3 uWindDirection;

void ApplyWind(inout vec3 position, vec3 normal, vec3 originalPosition)
{
    // Calculate the effect of wind using the original position in object space
    float windEffect = sin(dot(originalPosition, uWindDirection) * 10.0 + uTime * SPEED) * INTENSITY;
    
    // Limit displacement to avoid very large displacements
    windEffect = clamp(windEffect, -0.1, 0.1);

    // Displace the position by the normal and the effect of the wind
    position += normal * windEffect;
}

void main()
{
    // Positions in object space (before multiplication by the camera matrix)
    vec3 originalPosition0 = gl_in[0].gl_Position.xyz;
    vec3 originalPosition1 = gl_in[1].gl_Position.xyz;
    vec3 originalPosition2 = gl_in[2].gl_Position.xyz;

    // First
    vec3 position0 = gl_in[0].gl_Position.xyz;
    ApplyWind(position0, data_in[0].Normal, originalPosition0);    // Apply wind
    gl_Position = data_in[0].CameraMatrix * vec4(position0, 1.0);  // Final transformation
    vNormal = data_in[0].Normal;
    vColour = data_in[0].Colour;
    vTex = data_in[0].Tex;
    vFragmentPosition = data_in[0].FragmentPosition;
    vFragmentPositionLight = data_in[0].FragmentPositionLight;
    EmitVertex();

    // Second
    vec3 position1 = gl_in[1].gl_Position.xyz;
    ApplyWind(position1, data_in[1].Normal, originalPosition1);    // Apply wind
    gl_Position = data_in[1].CameraMatrix * vec4(position1, 1.0);  // Final transformation
    vNormal = data_in[1].Normal;
    vColour = data_in[1].Colour;
    vTex = data_in[1].Tex;
    vFragmentPosition = data_in[1].FragmentPosition;
    vFragmentPositionLight = data_in[1].FragmentPositionLight;
    EmitVertex();

    // Third
    vec3 position2 = gl_in[2].gl_Position.xyz;
    ApplyWind(position2, data_in[2].Normal, originalPosition2);    // Apply wind
    gl_Position = data_in[2].CameraMatrix * vec4(position2, 1.0);  // Final transformation
    vNormal = data_in[2].Normal;
    vColour = data_in[2].Colour;
    vTex = data_in[2].Tex;
    vFragmentPosition = data_in[2].FragmentPosition;
    vFragmentPositionLight = data_in[2].FragmentPositionLight;
    EmitVertex();

    EndPrimitive();
}

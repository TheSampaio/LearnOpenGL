// Sets the GLSL's version and compatibility mode
#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

// Output variables
out vec4 vColour;
out vec3 vNormal;
out vec2 vTex;
out vec3 vFragmentPosition;
out mat4 vCameraMatrix;

in DATA
{
	vec4 Colour;
	vec3 Normal;
	vec2 Tex;
    vec3 FragmentPosition;
    mat4 CameraMatrix;

} data_in[];

void main()
{
    // First
    gl_Position = data_in[0].CameraMatrix * gl_in[0].gl_Position;
    vNormal = data_in[0].Normal;
    vColour = data_in[0].Colour;
    vTex = data_in[0].Tex;
    vFragmentPosition = data_in[0].FragmentPosition;
    EmitVertex();

    // Second
    gl_Position = data_in[1].CameraMatrix * gl_in[1].gl_Position;
    vNormal = data_in[1].Normal;
    vColour = data_in[1].Colour;
    vTex = data_in[1].Tex;
    vFragmentPosition = data_in[1].FragmentPosition;
    EmitVertex();

    // Third
    gl_Position = data_in[2].CameraMatrix * gl_in[2].gl_Position;
    vNormal = data_in[2].Normal;
    vColour = data_in[2].Colour;
    vTex = data_in[2].Tex;
    vFragmentPosition = data_in[2].FragmentPosition;
    EmitVertex();

    EndPrimitive();
}

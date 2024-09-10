#version 330 core

in vec3 vTex;
out vec4 oFragment;

uniform samplerCube uSkybox;

void main()
{
	oFragment = texture(uSkybox, vTex);
}

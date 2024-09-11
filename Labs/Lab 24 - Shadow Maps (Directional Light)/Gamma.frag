#version 330 core

in vec2 vTex;
out vec4 oFragment;

uniform float uGamma;
uniform sampler2D uSurface;

void main()
{
	vec4 surface = texture(uSurface, vTex);
	oFragment.rgb = pow(surface.rgb, vec3(1.0 / uGamma));
}

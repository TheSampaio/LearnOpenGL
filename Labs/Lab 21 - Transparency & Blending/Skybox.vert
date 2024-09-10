#version 330 core

layout(location = 0) in vec3 iPosition;

out vec3 vTex;

uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
	vec4 position = uProjection * uView * vec4(iPosition, 1.0);
	vTex = vec3(iPosition.xy, -iPosition.z);
	gl_Position = vec4(position.xy, position.w, position.w);
}

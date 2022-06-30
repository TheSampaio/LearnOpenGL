#version 330 core

layout(location = 0) in vec3 aPos;

out vec3 texCoord;

uniform mat4 projection;
uniform mat4 view;

void main()
{
	vec4 position = projection * view * vec4(aPos, 1.0);
	texCoord = vec3(aPos.x, aPos.y, -aPos.z);
	gl_Position = vec4(position.x, position.y, position.w, position.w);;
}
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

uniform mat4 camMatrix;   // Define a uniform variable to the camera matrix function
uniform mat4 model;		  // Define a uniform variable to the model
uniform mat4 translation; // Define a uniform variable model's tranform
uniform mat4 rotation;	  // Define a uniform variable model's rotation
uniform mat4 scale;		  // Define a uniform variable model's scale
uniform float outlining;

void main()
{
	vec3 currentPos = vec3(model * translation * rotation * scale * vec4(aPos + aNormal * outlining, 1.0));
	gl_Position = camMatrix * vec4(currentPos, 1.0);
}
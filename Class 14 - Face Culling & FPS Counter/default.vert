#version 330 core // Definig GLSL's version

layout (location = 0) in vec3 aPos;	   // Define buffer's layout position and defing the begin as slot 0
layout (location = 1) in vec3 aNormal; // Define buffer's layout normal vertices and defing the begin as slot 1
layout (location = 2) in vec3 aColor;  // Define buffer's layout color and defing the begin as slot 2
layout (location = 3) in vec2 aTex;    // Define buffer's layout texture and defing the begin as slot 3

out vec3 currentPos;  // Define a variable to store and export the camera's coordinates to the Fragment Shader
out vec3 normalCoord; // Define a variable to store and export the normal's coordinates to the Fragment Shader
out vec3 color;		  // Define a variable to store and export the color's data to the Fragment Shader
out vec2 texCoord;	  // Define a variable to store and export the texture's coordinates to the Fragment Shader

uniform mat4 model;		// Define a uniform variable to the model
uniform mat4 camMatrix;	// Define a uniform variable to the camera matrix function

uniform mat4 translation; // Define a uniform variable model's tranform
uniform mat4 rotation;	  // Define a uniform variable model's rotation
uniform mat4 scale;		  // Define a uniform variable model's scale

// PS: NEVER declare uniforms if you won't use them

void main()
{
	currentPos = vec3(model * translation * -rotation * scale * vec4(aPos, 1.0f)); // Storing vertice's coordinates to export

	normalCoord = aNormal;						     // Storing normal's coordinates to export
	color = aColor;								     // Storing color's data to export
	texCoord = mat2(0.0f, -1.0f, 1.0f, 0.0f) * aTex; // Storing texture's coordinates to export

	gl_Position = camMatrix * vec4(currentPos, 1.0f); // Applying the vertice's positions
}
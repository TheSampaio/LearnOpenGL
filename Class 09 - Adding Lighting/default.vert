#version 330 core // Definig GLSL's version

layout (location = 0) in vec3 aPos;	   // Define buffer's layout position and defing the begin as slot 0
layout (location = 1) in vec3 aColor;  // Define buffer's layout color and defing the begin as slot 1
layout (location = 2) in vec2 aTex;    // Define buffer's layout texture and defing the begin as slot 2
layout (location = 3) in vec3 aNormal; // Define buffer's layout normal vertices and defing the begin as slot 3

out vec3 color;	   // Define a variable to store and export the color to the Fragment Shader
out vec2 texCoord; // Define a variable to store and export the textures coordinates to the Fragment Shader
out vec3 normalCoord;
out vec3 currentPos;

uniform mat4 pyramidModel;  // Define a uniform variable to the model
uniform mat4 camMatrix;		// Define a uniform variable to the camera matrix function

// PS: NEVER declare uniforms if you won't use them

void main()
{
	currentPos = vec3(pyramidModel * vec4(aPos, 1.0f));
	gl_Position = camMatrix * pyramidModel * vec4(currentPos, 1.0f); // Applying the vertice's positions

	color = aColor;		   // Storing colors data to export
	texCoord = aTex;	   // Storing texture coordinates to export
	normalCoord = aNormal; //
}
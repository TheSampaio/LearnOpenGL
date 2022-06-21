#version 330 core // Definig GLSL's version

layout (location = 0) in vec3 aPos;	  // Define buffer's layout position and defing the begin as slot 0
layout (location = 1) in vec3 aColor; // Define buffer's layout color and defing the begin as slot 1
layout (location = 2) in vec2 aTex;   // Define buffer's layout texture and defing the begin as slot 2

out vec3 color;	   // Define a variable to store and export the color to the Fragment Shader
out vec2 texCoord; // Define a variable to store and export the textures coordinates to the Fragment Shader

uniform float scale;	 // Define a uniform variable to multiply out positions (Scaling)
uniform mat4 model;		 // Define a uniform variable to the model
uniform mat4 view;		 // Define a uniform variable to the view
uniform mat4 projection; // Define a uniform variable to the projection

// PS: NEVER declare uniforms if you won't use them

void main()
{
   gl_Position = projection * view * model * vec4(aPos, 1.0); // Applying the vertice's positions

   color = aColor;	// Storing colors data to export
   texCoord = aTex;	// Storing texture coordinates to export
}
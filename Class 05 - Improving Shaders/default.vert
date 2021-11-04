#version 330 core // Definig the shader's version

layout (location = 0) in vec3 aPos;		// Definig the position's buffer layout and defing the begin as slot 0
layout (location = 1) in vec3 aColor;	// Definig the color's buffer layout and defing the begin as slot 1

out vec3 color;			// Defining a variable to store and export the color to the Fragment Shader
uniform float scale;	// Defining a uniform variable to multiply out positions (Scaling)

// PS: NEVER declare uniforms if you won't use them

void main()
{
   gl_Position = vec4(aPos.x + aPos.x * scale, aPos.y + aPos.y * scale, aPos.z + aPos.z * scale, 1.0);	// Applying the vertice's positions
   color = aColor;																						// Storing the colors to export
}
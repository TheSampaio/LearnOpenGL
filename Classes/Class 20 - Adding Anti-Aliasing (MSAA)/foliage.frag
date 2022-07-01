#version 330 core // Definig GLSL's version

in vec3 currentPos;  // Importing camera's position variable from vertex shader
in vec3 normalCoord; // Importing normal's variable from vertex shader
in vec3 color;	     // Importing color variable from vertex shader
in vec2 texCoord;    // Importing texture's coordinates from vertex shader

out vec4 FragColor; // Definig a variable to store colors data to export it

uniform sampler2D diffuse0; // Defining a uniform variable to use texture's coordinates from the buffer data (Diffuse)
uniform sampler2D specular0; // Defining a uniform variable to use texture's coordinates from the buffer data (Specular)
uniform vec4 lightColor; // Defining a uniform variable to change light's colors
uniform vec3 lightPos;   // Defining a uniform variable to store light's position
uniform vec3 camPos;	 // Defining a uniform variable to store camera's position

vec4 DirectionalLight() // Create a directional light
{
	// Ambient light
	float ambientLight = 0.5;

	// Difuse light
	vec3 normal = normalize(normalCoord);
	vec3 lightDirection = normalize(vec3(-0.5, 0.5, 1.0));
	float diffuse = max(abs(dot(normal, lightDirection)), 0.0); // Absolute helps with grass shading

	// Specular light
	float specularLight = 0.50;
	vec3 viewDirection = normalize(camPos - currentPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specularAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0), 4);
	float specular = specularAmount * specularLight;

	if (texture(diffuse0, texCoord).a < 0.1)
		discard;

	// Exporting the imported texture to show it on screen
	return (texture(diffuse0, texCoord) * (diffuse + ambientLight) + texture(specular0, texCoord).r * specular) * lightColor;
}

float near =  0.002;
float far =  40.000;
uniform vec3 fogColor;

float linearizeDepth(float depth)
{
	return (2.0 * near * far) / (far + near - (depth * 2.0 - 1.0) * (far - near));
}

float logisticDepth(float depth, float steepness = 0.5, float offset = 5.0)
{
	float zValue = linearizeDepth(depth);
	return (1 / (1 + exp(-steepness * (zValue - offset))));
}

void main()
{
	// Output final light's type and color
	float depth = logisticDepth(gl_FragCoord.z);
	FragColor = DirectionalLight() * (1.0 - depth) + vec4(depth * fogColor, 1.0);
}
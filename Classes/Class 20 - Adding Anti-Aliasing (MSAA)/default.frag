#version 330 core // Definig GLSL's version

in vec3 currentPos;  // Importing camera's position variable from vertex shader
in vec3 normalCoord; // Importing normal's variable from vertex shader
in vec3 color;	     // Importing color variable from vertex shader
in vec2 texCoord;    // Importing texture's coordinates from vertex shader

out vec4 FragColor; // Definig a variable to store colors data to export it

uniform sampler2D diffuse0;  // Defining a uniform variable to use texture's coordinates from the buffer data (Diffuse)
uniform sampler2D specular0; // Defining a uniform variable to use texture's coordinates from the buffer data (Specular)
uniform vec4 lightColor;	 // Defining a uniform variable to change light's colors
uniform vec3 lightPos;		 // Defining a uniform variable to store light's position
uniform vec3 camPos;		 // Defining a uniform variable to store camera's position

vec4 PointLight() // Create a point light
{
	// Point light variables
	vec3 lightVec = lightPos - currentPos;
	float distance = length(lightVec);
	float quadraticTerm = 0.4;
	float linearTerm = 0.02;
	float intensity = 1.0 / (quadraticTerm * distance * distance + linearTerm * distance + 1.0);

	// Ambient light
	float ambientLight = 0.20;

	// Difuse light
	vec3 normal = normalize(normalCoord);
	vec3 lightDirection = normalize(lightVec);
	float diffuse = max(dot(normal, lightDirection), 0.0);

	// Specular light
	float specularLight = 0.50;
	vec3 viewDirection = normalize(camPos - currentPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specularAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0), 12);
	float specular = specularAmount * specularLight;

	// Exporting the imported texture to show it on screen
	return (texture(diffuse0, texCoord) * (diffuse * intensity + ambientLight) + texture(specular0, texCoord).r * specular * intensity) * lightColor;
}

vec4 DirectionalLight() // Create a directional light
{
	// Ambient light
	float ambientLight = 0.5;

	// Difuse light
	vec3 normal = normalize(normalCoord);
	vec3 lightDirection = normalize(vec3(0.5, 0.5, -1.0));
	float diffuse = max(dot(normal, lightDirection), 0.0);

	// Specular light
	float specularLight = 0.50;
	vec3 viewDirection = normalize(camPos - currentPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specularAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0), 4);
	float specular = specularAmount * specularLight;

	// Exporting the imported texture to show it on screen
	return (texture(diffuse0, texCoord) * (diffuse + ambientLight) + texture(specular0, texCoord).r * specular) * lightColor;
}

vec4 SpotLight() // Create a spot light
{
	// Spot light variables
	float innerCone = 0.95;
	float outerCone = 0.80;
	vec3 lightVec = lightPos - currentPos;

	// Ambient light
	float ambientLight = 0.20;

	// Difuse light
	vec3 normal = normalize(normalCoord);
	vec3 lightDirection = normalize(lightVec);
	float diffuse = max(dot(normal, lightDirection), 0.0);

	// Specular light
	float specularLight = 0.50;
	vec3 viewDirection = normalize(camPos - currentPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specularAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0), 4);
	float specular = specularAmount * specularLight;

	float angle = dot(vec3(0.0, -1.0, 0.0), -lightDirection);
	float intensity = clamp((angle - outerCone) / (innerCone - outerCone), 0.0, 1.0);

	// Exporting the imported texture to show it on screen
	return (texture(diffuse0, texCoord) * (diffuse * intensity + ambientLight) + texture(specular0, texCoord).r * specular * intensity) * lightColor;
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
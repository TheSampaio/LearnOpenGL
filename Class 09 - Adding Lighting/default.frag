#version 330 core // Definig GLSL's version

in vec3 color;	   // Importing color variable from vertex shader
in vec2 texCoord;  // Importing texture's coordinates from vertex shader
in vec3 normalCoord;
in vec3 currentPos;

out vec4 FragColor; // Definig a variable to store colors data to export it

uniform sampler2D tex0;  // Defining a uniform variable to use texture's coordinates from the buffer data
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

void main()
{
	vec3 normal = normalize(normalCoord);
	vec3 lightDirection = normalize(lightPos - currentPos);

	float ambientLight = 0.2f;
	float diffuse = max(dot(normal, lightDirection), 0.0f);
	float specularLight = 0.50f;

	vec3 viewDirection = normalize(camPos - currentPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);

	float specularAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8);
	float specular = specularAmount * specularLight;

	FragColor = texture(tex0, texCoord) * lightColor * (diffuse + ambientLight + specular); // Exporting the imported texture to show it on screen
}
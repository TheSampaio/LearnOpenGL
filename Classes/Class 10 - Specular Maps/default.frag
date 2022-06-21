#version 330 core // Definig GLSL's version

in vec3 color;	     // Importing color variable from vertex shader
in vec2 texCoord;    // Importing texture's coordinates from vertex shader
in vec3 normalCoord; // Importing normal's variable from vertex shader
in vec3 currentPos;  // Importing camera's position variable from vertex shader

out vec4 FragColor; // Definig a variable to store colors data to export it

uniform sampler2D tex0; // Defining a uniform variable to use texture's coordinates from the buffer data (Diffuse)
uniform sampler2D tex1; // Defining a uniform variable to use texture's coordinates from the buffer data (Specular)

uniform vec4 lightColor; // Defining a uniform variable to change light's colors
uniform vec3 lightPos;   // Defining a uniform variable to store light's position
uniform vec3 camPos;	 // Defining a uniform variable to store camera's position

void main()
{
	vec3 normal = normalize(normalCoord);
	vec3 lightDirection = normalize(lightPos - currentPos);

	float ambientLight = 0.2f;
	float diffuse = max(dot(normal, lightDirection), 0.0f);
	float specularLight = 0.50f;

	vec3 viewDirection = normalize(camPos - currentPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);

	float specularAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 12);
	float specular = specularAmount * specularLight;

	FragColor = texture(tex0, texCoord) * lightColor * (diffuse + ambientLight) + texture(tex1, texCoord).r * specular; // Exporting the imported texture to show it on screen
}
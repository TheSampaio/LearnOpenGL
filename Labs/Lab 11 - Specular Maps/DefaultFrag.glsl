// Sets the GLSL's version and compatibility mode
#version 330 core

// Input variables
in vec4 vColour;
in vec3 vNormal;
in vec2 vTex;
in vec3 vFragmentPosition;

// Output variables
out vec4 oFragment;

// Structs
struct Light
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct LightDirection
{
    Light base;
    vec3 direction;
};

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float intensity;
    float shininess;
};

// Uniforms
uniform LightDirection uLightDirectional;
uniform Material uMaterial;
uniform vec3 uViewPosition;

// Function's prototypes
vec4 LightingDirectional(LightDirection light, vec3 normal, vec3 viewDirection);

void main()
{
    vec3 normal = normalize(vNormal);
    vec3 viewDirection = normalize(uViewPosition - vFragmentPosition);
    vec4 result = LightingDirectional(uLightDirectional, normal, viewDirection);

    // Final fragment colour
    oFragment = result;
}

vec4 LightingDirectional(LightDirection light, vec3 normal, vec3 viewDirection)
{
    // Ambient
    vec4 ambient = vec4(light.base.ambient, 1.0) * texture(uMaterial.diffuse, vTex) * vColour;

    // Diffuse
    vec3 direction = normalize(light.direction);
    float diffuseFactor = max(dot(normal, direction), 0.0);
    vec4 diffuse = vec4(light.base.diffuse, 1.0) * diffuseFactor * (texture(uMaterial.diffuse, vTex) * vColour);

    // Specular
    vec3 reflectDirection = reflect(-direction, normal);
    float specularFactor = pow(max(dot(viewDirection, reflectDirection), 0.0), uMaterial.shininess);
    vec4 specular = vec4(light.base.specular, 1.0) * specularFactor * texture(uMaterial.specular, vTex).r * uMaterial.intensity;

    // Output
    return ambient + diffuse + specular;
}

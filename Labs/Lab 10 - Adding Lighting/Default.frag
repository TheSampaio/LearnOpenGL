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

// Uniforms
uniform LightDirection uLightDirectional;
uniform sampler2D uDiffuseSampler;
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
    vec4 materialDiffuse = texture(uDiffuseSampler, vTex) * vColour;

    // Ambient
    vec4 ambient = vec4(light.base.ambient, 1.0) * materialDiffuse;

    // Diffuse
    vec3 direction = normalize(light.direction);
    float diffuseFactor = max(dot(normal, direction), 0.0);
    vec4 diffuse = vec4(light.base.diffuse, 1.0) * diffuseFactor * materialDiffuse;

    // Specular
    vec3 reflectDirection = normalize(reflect(-direction, normal));
    float specularFactor = pow(max(dot(viewDirection, reflectDirection), 0.0), 1.0);
    vec4 specular = vec4(light.base.specular, 1.0) * specularFactor * materialDiffuse.r * 0.20;

    // Output
    return ambient + diffuse + specular;
}

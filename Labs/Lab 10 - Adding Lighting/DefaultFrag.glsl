// Sets the GLSL's version and compatibility mode
#version 330 core

// Input variables
in vec4 vColour;
in vec3 vNormal;
in vec2 vUV;
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
uniform LightDirection uLightDirectionalSource;
uniform sampler2D uDiffuseSampler;
uniform vec3 uViewPosition;

// Function's prototypes
vec4 LightingDirectional(LightDirection LightSource, vec3 Normal, vec3 ViewDirection);

void main()
{
    vec3 Normal = normalize(vNormal);
    vec3 ViewDirection = normalize(uViewPosition - vFragmentPosition);

    vec4 Result = LightingDirectional(uLightDirectionalSource, Normal, ViewDirection);

    // Final fragment colour
    oFragment = Result;
}

vec4 LightingDirectional(LightDirection LightSource, vec3 Normal, vec3 ViewDirection)
{
    vec4 MaterialDiffuse = texture(uDiffuseSampler, vUV) * vColour;

    // Ambient
    vec4 Ambient = vec4(LightSource.base.ambient, 1.0) * MaterialDiffuse;

    // Diffuse
    vec3 Direction = normalize(LightSource.direction);
    float DiffuseFactor = max(dot(Normal, Direction), 0.0);
    vec4 Diffuse = vec4(LightSource.base.diffuse, 1.0) * DiffuseFactor * MaterialDiffuse;

    // Specular
    vec3 ReflectDirection = normalize(reflect(-Direction, Normal));
    float SpecularFactor = pow(max(dot(ViewDirection, ReflectDirection), 0.0), 1.0);
    vec4 Specular = vec4(LightSource.base.specular, 1.0) * SpecularFactor * MaterialDiffuse.r * 0.20;

    // Output
    return Ambient + Diffuse + Specular;
}

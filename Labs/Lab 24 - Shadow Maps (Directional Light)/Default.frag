// Sets the GLSL's version and compatibility mode
#version 330 core

#define NR_LIGHT_POINTS 20
#define FG_DEPTH_NEAR 0.006
#define FG_DEPTH_FAR 100.0

// Input variables
in vec4 vColour;
in vec3 vNormal;
in vec2 vTex;
in vec3 vFragmentPosition;
in vec4 vFragmentPositionLight;

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

struct LightPoint
{
    Light base;
    vec3 position;
    float constant;
    float linear;
    float quadratic;
};

struct LightSpot
{
    Light base;
    vec3 position;
    vec3 direction;
    float inner;
    float outer;
};

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float intensity;
    float shininess;
};

// Uniforms
uniform sampler2D uShadowMap;
uniform vec3 uViewPosition;
uniform Material uMaterial;

uniform int uMaxLightPoints;
uniform vec3 uBackgroundColour;
uniform LightDirection uLightDirectional;
uniform LightPoint uLightPoint[NR_LIGHT_POINTS];
uniform LightSpot uLightSpot;

// Function's prototypes
float DepthLinear(float depth);
float DepthLogistic(float depth, float steepness = 0.5, float offset = 5.0);
vec4 LightingDirectional(LightDirection light, vec3 normal, vec3 viewDirection);
vec4 LightingPoint(LightPoint light, vec3 normal, vec3 viewDirection, vec3 fragmentPosition);
vec4 LightingSpot(LightSpot light, vec3 normal, vec3 viewDirection, vec3 fragmentPosition);

void main()
{
    float depth = DepthLogistic(gl_FragCoord.z);
    vec3 normal = normalize(vNormal);
    vec3 viewDirection = normalize(uViewPosition - vFragmentPosition);

    vec4 result = LightingDirectional(uLightDirectional, normal, viewDirection);

    for (int i = 0; i < uMaxLightPoints; i++)
        result += LightingPoint(uLightPoint[i], normal, viewDirection, vFragmentPosition);

    result += LightingSpot(uLightSpot, normal, viewDirection, vFragmentPosition);

    // Discard alpha channel
    if (texture(uMaterial.diffuse, vTex).a < 0.2)
        discard;

    // Final fragment colour
    oFragment = result * (1.0 - depth) + vec4(depth * uBackgroundColour, 1.0);
}

float DepthLinear(float depth)
{
    return (2.0 * FG_DEPTH_NEAR * FG_DEPTH_FAR) / (FG_DEPTH_FAR + FG_DEPTH_NEAR - (depth * 2.0 - 1.0) * (FG_DEPTH_FAR - FG_DEPTH_NEAR));
}

float DepthLogistic(float depth, float steepness = 0.5, float offset = 5.0)
{
    float z = DepthLinear(depth);
	return (1 / (1 + exp(-steepness * (z - offset))));
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
    vec4 specular = vec4(0.0);

    if (diffuseFactor > 0.0)
    {
        vec3 halfWayDirection =  normalize(viewDirection + direction);
        float specularFactor = pow(max(dot(normal, halfWayDirection), 0.0), uMaterial.shininess);
        specular = vec4(light.base.specular, 1.0) * specularFactor * (texture(uMaterial.specular, vTex).r * uMaterial.intensity);
    }

    float shadow = 0.0;
    vec3 lightCoords = vFragmentPositionLight.xyz / vFragmentPositionLight.w;

    if (lightCoords.z <= 1.0)
    {
        lightCoords = (lightCoords + 1.0) / 2.0;

        float closestDepth = texture(uShadowMap, lightCoords.xy).r;
        float currentDepth = lightCoords.z;
        float bias = 0.00008;

        // Smoothens out the shadows
		int samples = 2;
		vec2 pixelSize = 1.0 / textureSize(uShadowMap, 0);

		for(int y = -samples; y <= samples; y++)
		{
		    for(int x = -samples; x <= samples; x++)
		    {
		        float closestDepth = texture(uShadowMap, lightCoords.xy + vec2(x, y) * pixelSize).r;

				if (currentDepth > closestDepth + bias)
					shadow += 1.0;     
		    }    
		}

		// Get average shadow
		shadow /= pow((samples * 2 + 1), 2);
    }

    // Output
    return ambient + diffuse * (1.0 - shadow) + specular * (1.0 - shadow);
}

vec4 LightingPoint(LightPoint light, vec3 normal, vec3 viewDirection, vec3 fragmentPosition)
{
    // Ambient
    vec4 ambient = vec4(light.base.ambient, 1.0) * texture(uMaterial.diffuse, vTex) * vColour;
    
    // Diffuse
    vec3 direction = normalize(light.position - fragmentPosition);
    float diffuseFactor = max(dot(normal, direction), 0.0);
    vec4 diffuse = vec4(light.base.diffuse, 1.0) * diffuseFactor * (texture(uMaterial.diffuse, vTex) * vColour);

    // Specular
    vec4 specular = vec4(0.0);

    if (diffuseFactor > 0.0)
    {
        vec3 halfWayDirection =  normalize(viewDirection + direction);
        float specularFactor = pow(max(dot(normal, halfWayDirection), 0.0), uMaterial.shininess);
        specular = vec4(light.base.specular, 1.0) * specularFactor * (texture(uMaterial.specular, vTex).r * uMaterial.intensity);
    }

    float lightDistance = length(light.position - fragmentPosition);
    float lightAttenuation = 1.0 / (light.constant + light.linear * lightDistance + light.quadratic * (lightDistance * lightDistance));

    ambient  *= lightAttenuation; 
    diffuse  *= lightAttenuation;
    specular *= lightAttenuation;

    // Output
    return ambient + diffuse + specular;
}

vec4 LightingSpot(LightSpot light, vec3 normal, vec3 viewDirection, vec3 fragmentPosition)
{
    // Ambient
    vec4 ambient = vec4(light.base.ambient, 1.0) * texture(uMaterial.diffuse, vTex) * vColour;
    
    // Diffuse
    vec3 direction = normalize(light.position - vFragmentPosition);
    float diffuseFactor = max(dot(normal, direction), 0.0);
    vec4 diffuse = vec4(light.base.diffuse, 1.0) * diffuseFactor * (texture(uMaterial.diffuse, vTex) * vColour);

    // Specular
    vec4 specular = vec4(0.0);

    if (diffuseFactor > 0.0)
    {
        vec3 halfWayDirection =  normalize(viewDirection + direction);
        float specularFactor = pow(max(dot(normal, halfWayDirection), 0.0), uMaterial.shininess);
        specular = vec4(light.base.specular, 1.0) * specularFactor * (texture(uMaterial.specular, vTex).r * uMaterial.intensity);
    }

    float theta = dot(light.direction, -direction);
    float intensity = clamp((theta - light.outer) / light.inner, 0.0, 1.0);

    diffuse  *= intensity;
    specular *= intensity;

    // Output
    return ambient + diffuse + specular;
}

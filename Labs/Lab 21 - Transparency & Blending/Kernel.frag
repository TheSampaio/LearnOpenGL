#version 330 core

out vec4 oFragment;
in vec2 vTex;

uniform sampler2D uSurface;

const float offset_x = 1.0 / 800.0;  
const float offset_y = 1.0 / 800.0;  

vec2 offsets[9] = vec2[]
(
    vec2(-offset_x,  offset_y), vec2( 0.0,    offset_y),  vec2( offset_x,  offset_y),
    vec2(-offset_x,  0.0),      vec2( 0.0,    0.0),       vec2( offset_x,  0.0),
    vec2(-offset_x, -offset_y), vec2( 0.0f,   -offset_y), vec2( offset_x, -offset_y) 
);

float kernel[9] = float[]
(
    -1, -1, -1,
    -1,  9, -1,
    -1, -1, -1
);

void main()
{
    vec3 colour = vec3(0.0);

    for(int i = 0; i < 9; i++)
        colour += vec3(texture(uSurface, vTex.st + offsets[i])) * kernel[i];

    oFragment = vec4(colour, 1.0);
}

#version 330 core
in vec2 UV;
in vec3 NORM;
in vec4 VERT_COL;
out vec4 color;
uniform sampler2D image;
uniform int selected;

float random(vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123);
}

float squareNoise(vec2 st, float scale) {
    vec2 grid = floor(st * scale);
    return random(grid);
}

void main()
{
    vec4 sampled = mix(vec4(0.0,1.0,0.0,1.0), vec4(squareNoise(UV, 20)), 0.2)*VERT_COL*0.4;
    color = vec4(sampled.xyz, 1.0);
}

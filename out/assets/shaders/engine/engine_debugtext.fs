#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;

vec2 ConvertNormToMid(vec2 inn)
{
    return inn*2.0-vec2(1.0,1.0);
}

vec2 ConvertMidToNorm(vec2 inn)
{
    return (inn+vec2(1.0,1.0))/2.0;
}

void main()
{
    vec2 midTC = ConvertNormToMid(TexCoords);
    midTC *= 1.4;
    midTC = ConvertMidToNorm(midTC);
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, midTC).r);
    if (sampled.w > 0.0)
        color = vec4(1.0,1.0,1.0,1.0) * sampled;
    else
        color = vec4(1,1,1,0.1);
    //color = vec4(1.0,1.0,1.0,1.0);
}

#version 450 core

#define sampleCount 735

layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;
layout(r32f, binding = 0) uniform image2D imgOutput;
uniform vec2 samples[sampleCount];
uniform sampler2D image;
uniform float range;
uniform vec2 offset;
uniform vec2 size;

void main() {
    float value = 0.0;
    ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);

    ivec2 texSize = textureSize(image, 0);
    vec2 uv = ((vec2(texelCoord) + 0.5) / vec2(texSize))*size;
    value = texelFetch(image, texelCoord, 0).r;
    value -= 0.3;
    if (value < 0) value = 0.0;

    for (int i = 0; i < sampleCount; i++)
    {
        vec2 tsample = vec2(samples[i].x, 1.0-samples[i].y);
        float dist = distance(uv, tsample+offset);
        if (dist < range)
        {
            value += 0.8f*(dist*(1/range));
        }
    }

    value = min(value, 1.0);

    imageStore(imgOutput, texelCoord, vec4(value,0,0,0));
}
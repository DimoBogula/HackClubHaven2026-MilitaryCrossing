#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform sampler2D depth;

void main()
{
    color = mix(texture(image, TexCoords), texture(depth, TexCoords), 0.0);
}

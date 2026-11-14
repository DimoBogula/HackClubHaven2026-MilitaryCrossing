#version 330 core
in vec2 TexCoords;
out vec4 color;
uniform sampler2D image;

void main()
{
    vec4 sampled = texture(image, TexCoords);
    float value = sampled.r;
    vec2 pixel = TexCoords*512;
    color = vec4(0,0,0,1.0);
    if (mod(pixel.y, 16) < 2)
    {
        color = vec4(0,0.1,0,1.0);
    }
    if (mod(pixel.x, 16) < 2)
    {
        color = vec4(0,0.1,0,1.0);
    }
    if (mod(pixel.x, 4) < 2 && mod(pixel.y+2, 32) < 4)
    {
        color = vec4(0,0.2,0,1.0);
    }
    if (mod(pixel.y, 4) < 2 && mod(pixel.x+2, 32) < 4)
    {
        color = vec4(0,0.2,0,1.0);
    }

    color.y = min(color.y+value,1.0);
}

#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform sampler2D depth;
uniform float time;

void main()
{
    color = texture(image, TexCoords);
    if (texture(image, TexCoords).xyz == vec3(0.0))
      color = vec4(0.0);
}

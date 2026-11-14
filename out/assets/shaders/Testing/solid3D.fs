#version 330 core
out vec4 color;

uniform sampler2D tex;

in vec2 UV;
in vec3 NORM;
in vec4 VERT_COL;

void main()
{
    color = vec4(texture(tex, UV).xyz, 1.0)*vec4(VERT_COL.xyz, 1.0);
}

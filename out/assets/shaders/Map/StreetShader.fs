#version 330 core
out vec4 color;

in vec2 UV;
in vec3 NORM;
in vec4 VERT_COL;

float random(vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123);
}

float squareNoise(vec2 st, float scale) {
    vec2 grid = floor(st * scale);
    return random(grid);
}

float singleDottedLine(vec2 st, float posY, float height, float dotWidth, float spacing) {
    float lineY = step(posY, st.y) * step(st.y, posY + height);

    float periodX = dotWidth + spacing;
    float xCoord = mod(st.x, periodX);
    float lineX = step(xCoord, dotWidth);

    return lineY * lineX;
}

void main()
{
    float isLine = singleDottedLine(vec2(UV.y,UV.x), 0.5, 0.02, 0.03, 0.03);

    float noise = squareNoise(UV, 100.0);
    vec4 baseColor = vec4(0.2, 0.2, 0.23, 0.5) * noise;

    if (isLine == 1.0)
    {
        baseColor = vec4(1.0, 1.0, 1.0, 1.0) * noise;
    }

    color = baseColor * VERT_COL;
}
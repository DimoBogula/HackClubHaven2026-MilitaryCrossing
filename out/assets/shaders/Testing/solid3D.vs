#version 450 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 UV;
out vec3 NORM;
out vec4 VERT_COL;

void main()
{
    UV = uv;
    NORM = normal;
    gl_Position = projection * view * model * vec4(pos, 1.0);

    vec4 worldPos = model * vec4(pos, 1.0);
    vec3 norm = normalize(mat3(model) * normal);

    vec4 ambient = vec4(0.2, 0.2, 0.2, 1.0);
    vec4 diffuse = vec4(0.5, 0.5, 0.5, 1.0);
    vec4 specular = vec4(1.0, 1.0, 1.0, 1.0);
    float shininess = 100.0;

    vec4 light_position = vec4(1.0,1.0,1.0,0.0);
    vec4 light_color = vec4(1.0,1.0,1.0,1.0);

    vec3 lightDir = normalize(vec3(light_position) - vec3(worldPos));

    vec3 viewPos = vec3(0.0, 0.0, 3.0);
    vec3 viewDir = normalize(viewPos - vec3(worldPos));

    float nDotL = max(dot(norm, lightDir), 0.0);
    vec4 diffuseColor = diffuse * light_color * nDotL;

    vec4 specularColor = vec4(0.0);
    if (nDotL > 0.0)
    {
        vec3 reflectDir = reflect(-lightDir, norm);
        float rDotV = max(dot(reflectDir, viewDir), 0.0);
        float specularStrength = pow(rDotV, shininess);
        specularColor = specular * light_color * specularStrength;
    }

    vec4 color = ambient + diffuseColor + specularColor;
    VERT_COL = color;
}
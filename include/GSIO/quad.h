#pragma once
#include <GL/glm/glm.hpp>
class GSIO_shaderasset;
class GSIO_texture;
class GSIO;
class GSIO_renderContext;
class GSIO_material;

class GSIO_quad
{
    public:
    unsigned int VAO = 0, VBO = 0;
    glm::vec2 position;
    glm::vec2 size;
    glm::vec4 uv = glm::vec4(0.0f,0.0f,1.0f,1.0f);
    bool centered = false;

    void BuildMesh();

    void Render(GSIO_shaderasset* shaderAsset, GSIO *gsio);

    GSIO_quad();
    ~GSIO_quad();
};
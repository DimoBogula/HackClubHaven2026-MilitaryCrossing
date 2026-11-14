#pragma once
#include <vector>
#include <string>
#include <variant>
#include <array>
#include "shader.h"
#include "texture.h"
#include "quad.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GL/glm/glm.hpp>
#include "uitypes.h"

class GSIO;

class GSIO_transform
{
    public:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 size;

    glm::mat4 Matrix();
};

class GSIO_vertex
{
    public:
    glm::vec3 position;
    glm::vec2 uv;
    glm::vec3 normal;
    uint8_t materialIndex;
};

struct GSIO_glMeshPair
{
    unsigned int VBO = 0, VAO = 0, EBO = 0, VertCount = 0, IndexCount = 0;

    ~GSIO_glMeshPair();
};

class GSIO_mesh
{
    public:
    std::vector<GSIO_vertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<GSIO_glMeshPair> glMeshes;

    void GenerateMesh();
};

enum class GSIO_uniformFormat
{
    Float,
    Int,
    Float2,
    Float3,
    Matrix4,
    Float2V,
    Float3V,
    FloatV,
    IntV,
    Automatic
};

#define GSIO_uniformType std::variant<float, int, std::array<float, 2>, std::array<float, 3>, glm::mat4, std::vector<std::array<float, 2>>, std::vector<std::array<float, 3>>, std::vector<float>, std::vector<int>>

class GSIO_uniform
{
    public:
    std::string name;
    unsigned int location = 0;
    GSIO_uniformFormat format;
    GSIO_uniformType value;
    bool locationUnassigned = true;

    void SetUniform(unsigned int shaderID);
};

class GSIO_material
{
    public:
    std::vector<GSIO_uniform> uniforms;
    std::vector<GSIO_texture*> textures;
    unsigned int shaderID;

    void BindTextures(uint8_t offset = 0);
    void SetUniforms();
    void SetUniform(std::string name, GSIO_uniformType value, GSIO_uniformFormat format = GSIO_uniformFormat::Automatic);
};

class GSIO_renderContext
{
    public:
    GSIO_texture* captureTexture = nullptr;
    GSIO_texture* depthTexture = nullptr;
    glm::mat4 view;
    glm::mat4 projection;
};

class GSIO_artist
{
    public:
    GSIO_transform transform;
    GSIO_mesh mesh;
    std::vector<GSIO_material> materials;
};

class GSIO_capturer
{
    public:
    GSIO_texture texture;
    GSIO_texture depthTexture;
    GSIO_color clearColor = {0.0f,0.0f,0.0f,1.0f};
    unsigned int FBO = 0, DFBO = 0;

    void Init(GSIO* gsio);
    void Bind();
    void Clear();
    void RenderArtist(GSIO_renderContext& rc, GSIO_artist* artist);
    void RenderQuad(GSIO_renderContext& rc, GSIO_quad* quad, GSIO_material* material, GSIO_transform* transform);
};

class GSIO_renderer
{
    public:
    GSIO_quad quad;
    GSIO_color clearColor = {0.0f,0.0f,0.0f,1.0f};
    std::vector<GSIO_shaderasset*> shaders;

    void Init(GSIO* gsio);

    void Render(GSIO* gsio, GSIO_renderContext& rc);
};
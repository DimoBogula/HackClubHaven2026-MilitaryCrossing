#pragma once
#include <map>
#include "shader.h"
#include "response.h"
#include "uitypes.h"
#include "drawing.h"
#include <GL/glm/glm.hpp>

class GSIO;
typedef char GLchar;

class GSIO_textfontcharacter
{
    public:
    unsigned int TextureID;
    glm::ivec2   Size;
    glm::ivec2   Bearing;
    unsigned int Advance;
};

class GSIO_textfont
{
    public:
    std::map<GLchar, GSIO_textfontcharacter> d_characters;
};

class GSIO_text
{
    public:
    std::string text = "";
    bool visible = true;
    unsigned int VAO = 0, VBO = 0;
    bool alive = false;

    GSIO_text(bool summon = false);
    ~GSIO_text();

    void Render(GSIO* gsio, GSIO_textfont* tf, GSIO_material* material, GSIO_transform* transform, GSIO_renderContext& rc);
};

GSIO_response GSIO_Load_Font(GSIO_textfont* tf, GSIO* gsio, std::string path);
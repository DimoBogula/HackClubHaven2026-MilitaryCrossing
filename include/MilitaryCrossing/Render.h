#pragma once
#include <GSIO/drawing.h>
#include <GSIO/text.h>

class GSIO_capturer;
class GSIO_renderer;
class GSIO_artist;

class GSIO;

class MC_RenderedText
{
    public:
    std::string text;
    GSIO_transform transform;
    GSIO_material* mat;
    GSIO_textfont* tf;
    GSIO_text textobj;

    void Init(GSIO_textfont* texf, GSIO* gsio);

    void Render(GSIO* gsio, GSIO_renderContext& rc);
};

struct MC_EasyRender
{
    public:
    GSIO_artist* model;
    GSIO_material material;

    void Load(GSIO* gsio, std::string matShader, std::string meshName);
    void Render(GSIO_capturer* view, GSIO* gsio, GSIO_renderContext &rc);
};

class MC_Renderer
{
    public:
    GSIO_capturer* playerView;
    GSIO_renderer* playerRenderer;

    GSIO_capturer* uiView;
    GSIO_renderer* uiRenderer;

    GSIO_artist* street;
    GSIO_material streetMaterial;

    GSIO_artist* streetgrass;
    GSIO_material streetgrassMaterial;

    MC_EasyRender test;

    MC_RenderedText moneyText;

    void Load(GSIO* gsio);
    void Update(GSIO* gsio);
};
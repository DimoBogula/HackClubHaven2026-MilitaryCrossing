#pragma once
#include <GSIO/drawing.h>
#include <GSIO/text.h>

class GSIO_capturer;
class GSIO_renderer;
class GSIO_artist;

class GSIO;

class MC_Renderer
{
    public:
    GSIO_capturer* playerView;
    GSIO_renderer* playerRenderer;
    GSIO_artist* street;
    GSIO_material streetMaterial;

    GSIO_text moneyText;

    void Load(GSIO* gsio);
    void Update(GSIO* gsio);
};
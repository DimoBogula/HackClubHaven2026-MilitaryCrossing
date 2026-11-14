#pragma once
#include <miniaudio.h>
#include <GSIO/drawing.h>
#include <GSIO/text.h>
#include <GSIO/audio.h>

class GSIO_capturer;
class GSIO_renderer;
class GSIO_artist;
class GSIO_texture;
class GSIO_quad;
class GSIO_shaderasset;
class GSIO_soundasset;
class GSIO_texturesheet;
class GSIO;

class GeeseOilCorpGlobalState
{
    public:
    ma_engine sound_engine;

    GSIO_capturer* playerView;
    GSIO_renderer* playerRenderer;
    GSIO_artist* sampleArtist;
    GSIO_material solidMaterial;
    GSIO_texture monkeyUV;
    GSIO_quad textureDisplay;
    GSIO_material textureMaterial;
    GSIO_texturesheet* sheet;
    GSIO_texture sheetTexture;
    GSIO_text text;
    GSIO_audioHost audioHost;
    GSIO_audioClip audioClip;
    GSIO_audioPlayer audioPlayer;

    long unsigned long frame = 0;

    void Init(GSIO* gsio);

    void Update(GSIO* gsio);
};
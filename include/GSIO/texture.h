#pragma once

enum class GSIO_textureFilter
{
    Linear,
    Pixel
};

class GSIO_imageData;

class GSIO_texture
{
    public:
    unsigned int texture = 0;
    unsigned int width = 0, height = 0;
    unsigned int format = 0, components = 0, componentType = 0;

    void InitTextureBase(GSIO_textureFilter filter, unsigned int activeTexture, unsigned int textureFormat, unsigned int textureComponents, unsigned int textureComponentType);
    void InitTexture(GSIO_textureFilter filter, unsigned int activeTexture, unsigned int textureFormat, unsigned int textureComponents, unsigned int textureComponentType);
    void InitTexture(GSIO_textureFilter filter, unsigned int activeTexture, GSIO_imageData* texasset);
    void Bind(unsigned int slot);

    ~GSIO_texture();
};
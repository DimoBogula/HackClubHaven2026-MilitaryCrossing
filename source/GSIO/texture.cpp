#include <GSIO/texture.h>
#include <GSIO/textureasset.h>
#include <GSIO/GSIO.h>

void GSIO_texture::InitTextureBase(GSIO_textureFilter filter, unsigned int activeTexture, unsigned int textureFormat, unsigned int textureComponents, unsigned int textureComponentType)
{
    glGenTextures(1, &texture);
    glActiveTexture(activeTexture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter == GSIO_textureFilter::Linear ? GL_LINEAR : GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter == GSIO_textureFilter::Linear ? GL_LINEAR : GL_NEAREST);
    format = textureFormat;
    components = textureComponents;
    componentType = textureComponentType;
}

void GSIO_texture::InitTexture(GSIO_textureFilter filter, unsigned int activeTexture, unsigned int textureFormat, unsigned int textureComponents, unsigned int textureComponentType)
{
    InitTextureBase(filter, activeTexture, textureFormat, textureComponents, textureComponentType);
    glTexImage2D(GL_TEXTURE_2D, 0, textureFormat, width, height, 0, textureComponents, textureComponentType, NULL);
    if (textureFormat == GL_DEPTH_COMPONENT32F) return;
    glBindImageTexture(0, texture, 0, GL_FALSE, 0, GL_READ_WRITE, textureFormat);
}

void GSIO_texture::InitTexture(GSIO_textureFilter filter, unsigned int activeTexture, GSIO_imageData *texasset)
{
    unsigned int textureFormat = GL_RGBA8, textureComponents = GL_RGBA, textureComponentType = GL_UNSIGNED_BYTE;
    InitTextureBase(filter, activeTexture, textureFormat, textureComponents, textureComponentType);
    width = texasset->width;
    height = texasset->height;
    glTexImage2D(GL_TEXTURE_2D, 0, textureFormat, width, height, 0, textureComponents, textureComponentType, texasset->image.data());
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindImageTexture(0, texture, 0, GL_FALSE, 0, GL_READ_WRITE, textureFormat);
}

void GSIO_texture::Bind(unsigned int slot)
{
    glActiveTexture(GL_TEXTURE0+slot);
    glBindTexture(GL_TEXTURE_2D, texture);
}

GSIO_texture::~GSIO_texture()
{
    if (texture != 0) glDeleteTextures(1, &texture);
}

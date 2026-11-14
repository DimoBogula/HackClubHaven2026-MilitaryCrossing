#pragma once
#include "asset.h"
#include "uitypes.h"
#include <GL/glm/glm.hpp>

#define GSIO_texture_cache_ext ".texsheetcache"
#define GSIO_texture_cache_folder GSIO_assetmanager_Default_Path_Cache "texsheets/"

class GSIO_texturesheetEntry
{
    public:
    glm::vec2 offset;
    unsigned int width, height;
    glm::vec4 uv;
    std::string fileName;
};

class GSIO_texturesheet : public GSIO_assetdata
{
    public:
    std::vector<GSIO_texturesheetEntry> entries;
    GSIO_imageData image;
    GSIO_response LoadData(GSIO_assetdatastructure assetData) override;
    GSIO_response CacheData(GSIO_assetdatastructure assetData) override;
};
#pragma once
#include "asset.h"
#include "uitypes.h"

#define GSIO_texture_cache_ext ".texturecache"
#define GSIO_texture_cache_folder GSIO_assetmanager_Default_Path_Cache "textures/"

class GSIO_textureasset : public GSIO_assetdata
{
    public:
    GSIO_imageData image;
    GSIO_response LoadData(GSIO_assetdatastructure assetData) override;
    GSIO_response CacheData(GSIO_assetdatastructure assetData) override;
};
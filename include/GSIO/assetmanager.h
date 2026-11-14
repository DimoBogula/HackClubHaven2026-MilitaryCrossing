#pragma once
#include "asset.h"

#include <vector>

#define GSIO_assetmanager_Default_Path_Cache "cache/"
#define GSIO_assetmanager_Default_Path_Assets "assets/"
#define GSIO_assetmanager_Default_File_Ext ".ini"

class GSIO;
class GSIO_shaderasset;
class GSIO_soundasset;
class GSIO_meshasset;
class GSIO_textureasset;
class GSIO_texturesheet;

class GSIO_assetmanager
{
    public:

    std::vector<GSIO_asset> assets;
    GSIO_response LoadAllAssets();
    GSIO_response CacheAllAssets();
    GSIO_response LoadAsset(std::string path);
    GSIO_response InitAsset(GSIO_asset& asset);
    bool AssetExists_Path(std::string path);

    GSIO_asset* GetAsset(std::string name);
    GSIO_shaderasset* GetShaderAsset(std::string name);
    GSIO_soundasset* GetSoundAsset(std::string name);
    GSIO_meshasset* GetMeshAsset(std::string name);
    GSIO_textureasset* GetTextureAsset(std::string name);
    GSIO_texturesheet* GetTexturesheetAsset(std::string name);

    GSIO_assetmanager() {}
};

GSIO_response GSIO_Init_assetmanager(GSIO_assetmanager* assm);
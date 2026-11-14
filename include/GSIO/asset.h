#pragma once

#include "response.h"
#include <ini.h>

enum class GSIO_assettype
{
    Shader,
    Font,
    Texture,
    Texturesheet,
    Sound,
    Model,
    Level,
    Binary,
    Text = Binary,
    Unknown
};

struct GSIO_assetdatastructure
{
    public:
    IniParser ini;
    GSIO_assettype type;
    std::string name;
    bool cache;
    bool tryLoadFromCache = false;

    std::string fullPath;

    GSIO_assetdatastructure() {}
    GSIO_assetdatastructure(std::string filePath);
};

class GSIO_assetdata
{
    public:
    virtual GSIO_response LoadData(GSIO_assetdatastructure assetData);
    virtual GSIO_response CacheData(GSIO_assetdatastructure assetData);
    virtual ~GSIO_assetdata() {}
};

class GSIO_asset
{
    public:

    GSIO_assetdatastructure assetData;
    GSIO_assetdata* data = nullptr;
    bool loaded = false;

    GSIO_asset() {}
    ~GSIO_asset();
};
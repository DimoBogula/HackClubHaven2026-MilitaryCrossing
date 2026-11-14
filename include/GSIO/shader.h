#pragma once
#include "asset.h"
typedef unsigned int GLuint;

#define GSIO_shader_cache_folder GSIO_assetmanager_Default_Path_Cache "shaders/"
#define GSIO_shader_cache_ext ".shadercache"

class GSIO_shaderasset : public GSIO_assetdata
{
    public:
    unsigned int ID;
    bool geometryShader = false;
    bool computeShader = false;
    bool vertexShader = false;
    bool fragmentShader = false;
    void use();
    GSIO_response CheckCompileErrors(GLuint shader, std::string type);
    GSIO_response LoadShaderFromCache(GSIO_assetdatastructure assetData);
    GSIO_response LoadShader(GSIO_assetdatastructure& assetData, unsigned int* ver, unsigned int* fra, unsigned int* geo, unsigned int* pro);
    GSIO_response LoadData(GSIO_assetdatastructure assetData) override;
    GSIO_response CacheData(GSIO_assetdatastructure assetData) override;
};
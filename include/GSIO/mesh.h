#pragma once
#include "asset.h"
#include "drawing.h"

class GSIO_meshasset : public GSIO_assetdata
{
    public:
    std::vector<GSIO_vertex> vertices;
    std::vector<uint32_t> indices;
    GSIO_response LoadData(GSIO_assetdatastructure assetData) override;
    //GSIO_response CacheData(GSIO_assetdatastructure assetData) override;
};
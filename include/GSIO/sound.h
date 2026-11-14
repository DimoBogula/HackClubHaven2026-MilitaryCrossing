#pragma once
#include "asset.h"
#include <miniaudio.h>
typedef unsigned int GLuint;

class GSIO_soundasset : public GSIO_assetdata
{
    public:
    float* pcm_data;
    ma_uint64 total_frames;
    ma_uint32 channels;
    ma_uint32 sample_rate;
    GSIO_response LoadData(GSIO_assetdatastructure assetData) override;
    //GSIO_response CacheData(GSIO_assetdatastructure assetData) override;
};
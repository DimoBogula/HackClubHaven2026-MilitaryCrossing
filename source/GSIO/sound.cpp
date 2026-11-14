#include <GSIO/sound.h>
#include <miniaudio.h>

GSIO_response GSIO_soundasset::LoadData(GSIO_assetdatastructure assetData)
{
    std::string filepath = assetData.fullPath+".mp3";
    ma_decoder decoder;
    if (ma_decoder_init_file(filepath.c_str(), NULL, &decoder) != MA_SUCCESS) {
        return {false, GSIO_response_Error_Prefix " - Failed to load sound."};
    }

    channels = decoder.outputChannels;
    sample_rate = decoder.outputSampleRate;
    
    ma_decoder_get_length_in_pcm_frames(&decoder, &total_frames);

    pcm_data = (float*)malloc(total_frames * channels * sizeof(float));

    ma_uint64 frames_read = 0;
    ma_decoder_read_pcm_frames(&decoder, pcm_data, total_frames, &frames_read);
    
    ma_decoder_uninit(&decoder);
    return {true};
}
#include <GSIO/audio.h>
#include <GSIO/sound.h>

GSIO_audioHost::GSIO_audioHost()
{
    ma_engine_init(NULL, &engine);
}

GSIO_audioHost::~GSIO_audioHost()
{
    ma_engine_uninit(&engine);
}

void GSIO_audioPlayer::LoadSoundPCM(GSIO_audioHost* host, GSIO_audioClip* clip)
{
    ma_sound_init_from_data_source(&host->engine, &clip->audio_buffer, 0, NULL, &sound);
}

void GSIO_audioPlayer::Play()
{
    ma_sound_start(&sound);
}

void GSIO_audioPlayer::Pause() {}

void GSIO_audioPlayer::Stop()
{
    ma_sound_stop(&sound);
}

void GSIO_audioPlayer::SetVolume(float v)
{
    ma_sound_set_volume(&sound, v);
}

float GSIO_audioPlayer::GetVolume()
{
    return ma_sound_get_volume(&sound);
}

void GSIO_audioPlayer::SetPitch(float v)
{
    ma_sound_set_pitch(&sound, v);
}

float GSIO_audioPlayer::GetPitch()
{
    return ma_sound_get_pitch(&sound);
}

void GSIO_audioPlayer::SetStereo(float v)
{
    ma_sound_set_pan(&sound, v);
}

float GSIO_audioPlayer::GetStereo()
{
    return ma_sound_get_pan(&sound);
}

GSIO_audioPlayer::~GSIO_audioPlayer()
{
    ma_sound_uninit(&sound);
}

void GSIO_audioClip::LoadPCM(float *pcm_data, uint64_t total_frames, uint32_t channels, uint32_t sample_rate)
{
    if (init)
    {
        ma_audio_buffer_uninit(&audio_buffer);
    }
    init = true;
    ma_audio_buffer_config bufferConfig = ma_audio_buffer_config_init(
        ma_format_f32,
        channels,
        total_frames,
        pcm_data,
        NULL
    );
    bufferConfig.sampleRate = sample_rate;
    ma_audio_buffer_init(&bufferConfig, &audio_buffer);
}

void GSIO_audioClip::Load(GSIO_soundasset *sa)
{
    LoadPCM(sa->pcm_data, sa->total_frames, sa->channels, sa->sample_rate);
}

GSIO_audioClip::~GSIO_audioClip()
{
    if (init)
        ma_audio_buffer_uninit(&audio_buffer);
}

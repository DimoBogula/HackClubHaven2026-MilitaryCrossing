#include <miniaudio.h>
#include <cstdint>
#include <GL/glm/vec3.hpp>

class GSIO_soundasset;

class GSIO_audioHost
{
    public:
    ma_engine engine;

    void SetVolume(float v);
    float GetVolume();

    GSIO_audioHost();
    ~GSIO_audioHost();
};

class GSIO_audioClip
{
    private:
    bool init = false;
    public:
    ma_audio_buffer audio_buffer;

    void LoadPCM(float* pcm_data, uint64_t total_frames, uint32_t channels, uint32_t sample_rate);
    void Load(GSIO_soundasset* sa);

    ~GSIO_audioClip();
};

class GSIO_audioPlayer
{
    public:
    ma_sound sound;

    void LoadSoundPCM(GSIO_audioHost* host, GSIO_audioClip* clip);
    void Play();
    void Pause();
    void Stop();
    void SetVolume(float v);
    float GetVolume();
    void SetPitch(float v);
    float GetPitch();
    void SetStereo(float v);
    float GetStereo();
    void SetLooping(bool v);
    bool GetLooping();

    ~GSIO_audioPlayer();
};
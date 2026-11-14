#pragma once
#include <GSIO/audio.h>
#include <cstdint>
#define TRACK_COUNT 6
#define TRACK_PREFIX "track"

class GSIO;

class MC_MusicState
{
    public:
    bool active = false;
    float timeSince = 0.0f;
};

class MC_Soundtrack
{
    public:

    GSIO_audioHost musicHost;
    GSIO_audioClip musicClips[TRACK_COUNT];
    GSIO_audioPlayer musicPlayers[TRACK_COUNT];
    MC_MusicState musicConfig[TRACK_COUNT];

    float fadeTimeMult = 0.3f;

    void Load(GSIO* gsio);
    void Update(GSIO* gsio);
    void ActivateTrack(uint8_t index);
    void DeactivateTrack(uint8_t index);
    void Test();
};
#pragma once
#include <GSIO/audio.h>
#define TRACK_COUNT 6
#define TRACK_PREFIX "track"

class GSIO;

class MC_Soundtrack
{
    public:

    GSIO_audioHost musicHost;
    GSIO_audioClip musicClips[TRACK_COUNT];
    GSIO_audioPlayer musicPlayers[TRACK_COUNT];

    void Load(GSIO* gsio);
    void Test();
};
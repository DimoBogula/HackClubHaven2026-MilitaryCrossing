#include <MilitaryCrossing/Soundtrack.h>
#include <GSIO/GSIO.h>

void MC_Soundtrack::Load(GSIO* gsio)
{
    for (int i = 0; i < TRACK_COUNT; i++)
    {
        musicClips[i].Load(gsio->assetmanager.GetSoundAsset(TRACK_PREFIX + std::to_string(i)));
        musicPlayers[i].LoadSoundPCM(&musicHost, &musicClips[i]);
    }
}

void MC_Soundtrack::Test()
{
    for (int i = 0; i < TRACK_COUNT; i++)
    {
        musicPlayers[i].Play();
    }
}

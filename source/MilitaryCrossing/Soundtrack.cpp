#include <MilitaryCrossing/Soundtrack.h>
#include <GSIO/GSIO.h>

void MC_Soundtrack::Load(GSIO* gsio)
{
    for (int i = 0; i < TRACK_COUNT; i++)
    {
        musicClips[i].Load(gsio->assetmanager.GetSoundAsset(TRACK_PREFIX + std::to_string(i)));
        musicPlayers[i].LoadSoundPCM(&musicHost, &musicClips[i]);
    }
    for (int i = 0; i < TRACK_COUNT; i++)
    {
        musicConfig[i].timeSince = 0.0f;
        musicConfig[i].active = false;
        musicPlayers[i].SetVolume(musicConfig[i].timeSince);
        musicPlayers[i].SetLooping(true);
        musicPlayers[i].Play();
    }
}

void MC_Soundtrack::Update(GSIO *gsio)
{
    for (int i = 0; i < TRACK_COUNT; i++)
    {
        musicConfig[i].timeSince += gsio->window.i_deltaTime * (musicConfig[i].active ? 1.0f : -1.0f) * fadeTimeMult;
        musicConfig[i].timeSince = std::min(std::max(musicConfig[i].timeSince, 0.0f), 1.0f);
        musicPlayers[i].SetVolume(musicConfig[i].timeSince);
    }
}

void MC_Soundtrack::ActivateTrack(uint8_t index)
{
    musicConfig[index].active = true;
}

void MC_Soundtrack::DeactivateTrack(uint8_t index)
{
    musicConfig[index].active = false;
}

void MC_Soundtrack::Test()
{
    for (int i = 0; i < TRACK_COUNT; i++)
    {
        musicPlayers[i].Play();
    }
}

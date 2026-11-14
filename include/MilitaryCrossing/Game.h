#pragma once
#include <cstdint>
#include <GSIO/audio.h>
#include <vector>

class MC_EasyRender;
class MC_RenderedText;

class GSIO;
class MC_Game;

class MC_Trooper
{
    public:
    std::vector<MC_EasyRender*> meshes;
    float health = 0.0f;
    float maxhealth = 1.0f;
    float price = 0.0f;
    float sellPrice = 0.0f;
    float timeSinceSell = 0.0f;
    float position = 0.0f;
    bool selected = false;

    uint16_t level;
    uint16_t nextUpgradePrice;

    MC_RenderedText* stats;

    GSIO_audioClip clip;
    GSIO_audioPlayer player;

    virtual void Load(GSIO* gsio, MC_Game* game) {}
    virtual void Update(GSIO* gsio, MC_Game* game) {}
};

class MC_BarricadeSoldier : public MC_Trooper
{
    public:
    void SetPos();
    void Load(GSIO* gsio, MC_Game* game) override;
    void Update(GSIO* gsio, MC_Game* game) override;
};

class MC_Game
{
    public:
    float timeSinceWave = 0.0f;
    uint64_t waveIndex = 0;

    int selectedTroopIndex = -1;

    bool left = false;
    bool right = false;

    uint64_t money = 0;
    std::vector<MC_Trooper*> troopers;

    void Init(GSIO* gsio);

    void Update(GSIO* gsio);
};
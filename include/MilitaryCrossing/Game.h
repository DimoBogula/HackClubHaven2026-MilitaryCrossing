#pragma once
#include <cstdint>
#include <GSIO/audio.h>
#include <vector>

class MC_EasyRender;
class MC_RenderedText;

class GSIO;
class MC_Game;

class MC_Enemy
{
    public:
    MC_EasyRender* mesh;
    float position;
    float xposition;
    float health = 0.0f;
    float gene = 0.0f;

    std::vector<GSIO_audioClip> clips;
    GSIO_audioClip deathClip;
    GSIO_audioPlayer player;

    bool die = false;

    void PlayRandomS(MC_Game* game);

    void Load(GSIO* gsio, MC_Game* game);
    void Update(GSIO* gsio, MC_Game* game);
};

class MC_Trooper
{
    public:
    std::vector<MC_EasyRender*> meshes;
    float health = 10.0f;
    uint64_t sellPrice = 0;
    float timeSinceBuy = 0.0f;
    float position = 0.0f;
    bool selected = false;

    float timer = 0.0f;

    uint16_t level;
    uint16_t nextUpgradePrice;

    MC_RenderedText* stats;

    GSIO_audioClip clip;
    GSIO_audioPlayer player;

    void TextSet(GSIO* gsio);

    void TextInit(GSIO* gsio);
    void TextUpdate(GSIO* gsio);

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
    uint64_t waveIndex = 1;

    int selectedTroopIndex = -1;

    bool left = false;
    bool right = false;

    uint64_t money = 0;
    std::vector<MC_Trooper*> troopers;
    std::vector<MC_Enemy*> enemies;

    GSIO_audioHost ahost;

    float ProgCurve(float x);

    void Init(GSIO* gsio);

    void Update(GSIO* gsio);
};
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

    bool die = false;

    void PlayRandomS(MC_Game* game, GSIO* gsio);

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

    bool die = false;

    GSIO_audioClip clips[4];
    GSIO_audioPlayer clipPlayers[4];
    GSIO_audioClip deathClip;
    GSIO_audioPlayer deathPlayer;

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

    bool dead = false;

    float timeSinceWave = 0.0f;
    uint64_t waveIndex = 1;

    uint64_t permaBonusCount = 0;

    GSIO_audioClip lvlUp;
    GSIO_audioPlayer lvlUpP;

    int selectedTroopIndex = -1;

    bool left = false;
    bool right = false;

    uint64_t money = 100;
    std::vector<MC_Trooper*> troopers;
    std::vector<MC_Enemy*> enemies;

    float ProgCurve(float x);

    void Init(GSIO* gsio);

    void Update(GSIO* gsio);
};
#include <MilitaryCrossing/Game.h>
#include <MilitaryCrossing/Render.h>
#include <MilitaryCrossing/Soundtrack.h>
#include <GSIO/GSIO.h>
#include <GSIO/drawing.h>
#include <algorithm>
#include <ctime>
#include <iostream>

void MC_BarricadeSoldier::SetPos()
{
    meshes[0]->model->transform.position.z = position-0.2f;
    meshes[1]->model->transform.position.z = position;
}

void MC_BarricadeSoldier::Load(GSIO *gsio, MC_Game *game)
{
    MC_EasyRender* soldier = new MC_EasyRender();
    soldier->Load(gsio, "SoldierSh", "BarricadeSoldier", "SoldierTex");
    MC_EasyRender* barricade = new MC_EasyRender();
    barricade->Load(gsio, "ConcreteBarricadeSh", "ConcreteBarricade", "ConcreteBarricadeTex");

    meshes.push_back(soldier);
    meshes.push_back(barricade);

    clip.Load(gsio->assetmanager.GetSoundAsset("shoot"));
    player.LoadSoundPCM(&gsio->state->soundtrack.musicHost, &clip);
    player.SetVolume(0.1f);

    for (int i = 0; i < 4; i++)
    {
        clips[i].Load(gsio->assetmanager.GetSoundAsset(std::string("a")+std::to_string(i)));
        clipPlayers[i].LoadSoundPCM(&gsio->state->soundtrack.musicHost, &clips[i]);
    }

    deathClip.Load(gsio->assetmanager.GetSoundAsset("endeath"));
    deathPlayer.LoadSoundPCM(&gsio->state->soundtrack.musicHost, &deathClip);

    TextInit(gsio);

    SetPos();
}

void MC_BarricadeSoldier::Update(GSIO *gsio, MC_Game *game)
{
    player.SetVolume(0.1f/(float)std::max(1,(int)game->troopers.size()));
    SetPos();
    TextUpdate(gsio);

    for (MC_Enemy* en : game->enemies)
    {
        if (en->position-position < 0.01f && en->health >= 0.0f)
        {
            game->permaBonusCount++;
            die = true;
        }
        int fireLevel = (int)floor((float)level / 4.0f)+1;
        if (en->position-position < 1.0f && timer > ((0.4f)/fireLevel) && en->health >= 0.0f)
        {
            en->health -= 1.0f+((level+1)*0.4f);
            if (en->health < 0.0f)
            {
                game->money += 10*(int)(std::max(en->gene, 1.0f))+(game->permaBonusCount*game->ProgCurve(game->waveIndex)*1.7f);
                ma_sound_seek_to_pcm_frame(&deathPlayer.sound, 0);
                deathPlayer.Play();
            }
            else
            {
                uint8_t index = rand() % 4;
                ma_sound_seek_to_pcm_frame(&clipPlayers[index].sound, 0);
                clipPlayers[index].Play();
            }
            timer = 0.0f;
            ma_sound_seek_to_pcm_frame(&player.sound, 0);
            player.Play();
            break;
        }
    }

    timer += gsio->window.i_deltaTime;
}

int MC_Game::EnemiesForWave(uint64_t wi)
{
    return (int)std::ceil(ProgCurve(wi));
}

float MC_Game::ProgCurve(float x)
{
    return 0.1f*std::pow(1.3f, x);
}

void MC_Game::Init(GSIO *gsio)
{
    srand(time(NULL));
    MC_BarricadeSoldier* bs = new MC_BarricadeSoldier();
    bs->Load(gsio, this);
    troopers.push_back(bs);
    MC_Enemy* en = new MC_Enemy();
    en->Load(gsio, this);
    enemies.push_back(en);

    lvlUp.Load(gsio->assetmanager.GetSoundAsset("lvlUp"));
    lvlUpP.LoadSoundPCM(&gsio->state->soundtrack.musicHost, &lvlUp);
}

void MC_Game::Update(GSIO *gsio)
{
    bool left = false;
    bool right = false;
    bool enter = false;

    bool trueLeft = glfwGetKey(gsio->window.r_window, GLFW_KEY_LEFT) == GLFW_PRESS;
    bool trueRight = glfwGetKey(gsio->window.r_window, GLFW_KEY_RIGHT) == GLFW_PRESS;

    if (trueLeft && lastLeft == GLFW_RELEASE)
        left = true;

    lastLeft = glfwGetKey(gsio->window.r_window, GLFW_KEY_LEFT);

    if (trueRight && lastRight == GLFW_RELEASE)
        right = true;

    lastRight = glfwGetKey(gsio->window.r_window, GLFW_KEY_RIGHT);

    if (glfwGetKey(gsio->window.r_window, GLFW_KEY_ENTER) == GLFW_PRESS && lastEnter == GLFW_RELEASE)
        enter = true;

    lastEnter = glfwGetKey(gsio->window.r_window, GLFW_KEY_ENTER);

    if (glfwGetKey(gsio->window.r_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        left = glfwGetKey(gsio->window.r_window, GLFW_KEY_LEFT) == GLFW_PRESS;
        right = glfwGetKey(gsio->window.r_window, GLFW_KEY_RIGHT) == GLFW_PRESS;
        enter = glfwGetKey(gsio->window.r_window, GLFW_KEY_ENTER) == GLFW_PRESS;
    }

    // dynamic music
    if (enemies.size() > 0)
    {
        gsio->state->soundtrack.ActivateTrack(2);
    }
    else
    {
        gsio->state->soundtrack.DeactivateTrack(2);
    }

    if (left)
        waveIndex--;
    else if (right)
        waveIndex++;

    // wave music
    if (waveIndex >= 5)
    {
        gsio->state->soundtrack.ActivateTrack(3);
    }
    else
    {
        gsio->state->soundtrack.DeactivateTrack(3);
    }
    if (waveIndex >= 10)
    {
        gsio->state->soundtrack.ActivateTrack(4);
    }
    else
    {
        gsio->state->soundtrack.DeactivateTrack(4);
    }
    if (waveIndex >= 15)
    {
        gsio->state->soundtrack.ActivateTrack(5);
    }
    else
    {
        gsio->state->soundtrack.DeactivateTrack(5);
    }

    if (dead)
    {
        if (enter)
        {
            dead = false;
            for (MC_Enemy* en : enemies) delete en;
            enemies.clear();
            for (MC_Trooper* t : troopers) delete t;
            troopers.clear();
            money = 100;
            waveIndex = 1;
            timeSinceWave = 0.0f;
            permaBonusCount = 0;
            selectedTroopIndex = -1;

            MC_BarricadeSoldier* bs = new MC_BarricadeSoldier();
            bs->Load(gsio, this);
            troopers.push_back(bs);
            MC_Enemy* en = new MC_Enemy();
            en->Load(gsio, this);
            enemies.push_back(en);
        }
        return;
    }

    if (!wavePause)
        timeSinceWave += gsio->window.i_deltaTime;
    
    if (enemies.size() == 0 && enter && glfwGetKey(gsio->window.r_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && wavePause)
    {
        wavePause = false;
        timeSinceWave = 0.0f;
        waveIndex++;
        for (int i = 0; i < EnemiesForWave(waveIndex); i++)
        {
            MC_Enemy* en = new MC_Enemy();
            en->Load(gsio, this);
            enemies.push_back(en);
        }
    }

    bool del = glfwGetKey(gsio->window.r_window, GLFW_KEY_BACKSPACE) == GLFW_PRESS;
    if (glfwGetKey(gsio->window.r_window, GLFW_KEY_UP) == GLFW_PRESS && troopers.size() > 0)
    {
        selectedTroopIndex = 0;
    }
    if (glfwGetKey(gsio->window.r_window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        selectedTroopIndex = -1;
    }

    if (selectedTroopIndex > -1 && troopers.size() == 0) selectedTroopIndex = -1;
    if (selectedTroopIndex >= 0)
    {
        if (right)
        {
            selectedTroopIndex++;
        }
        if (left)
        {
            selectedTroopIndex--;
        }
        if (selectedTroopIndex < 0) selectedTroopIndex = 0;
        if (selectedTroopIndex >= troopers.size()) selectedTroopIndex = troopers.size()-1;
        float dir = 0.0f;
        if (glfwGetKey(gsio->window.r_window, GLFW_KEY_A) == GLFW_PRESS) dir -= 1.0f;
        if (glfwGetKey(gsio->window.r_window, GLFW_KEY_D) == GLFW_PRESS) dir += 1.0f;
        troopers[selectedTroopIndex]->position += dir*0.6f*gsio->window.i_deltaTime;

        #define UPGRADE_COST 100

        if (enter && money >= UPGRADE_COST && glfwGetKey(gsio->window.r_window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
        {
            if (glfwGetKey(gsio->window.r_window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
            {
                uint64_t possibleUpgrades = (uint64_t)std::floor((float)money/(float)UPGRADE_COST);
                money -= possibleUpgrades*UPGRADE_COST;
                troopers[selectedTroopIndex]->level += possibleUpgrades;
            }
            else
            {
                troopers[selectedTroopIndex]->level++;
                money -= UPGRADE_COST;
            }
            lvlUpP.Play();
        }

        if (del)
        {
            money += troopers[selectedTroopIndex]->sellPrice;
            troopers[selectedTroopIndex]->die = true;
            selectedTroopIndex = -1;
        }
    }
    else
    {
        if (enter && money >= 150)
        {
            MC_BarricadeSoldier* bs = new MC_BarricadeSoldier();
            bs->Load(gsio, this);
            troopers.push_back(bs);
            money -= 150;
        }
    }
    for (MC_Trooper* t : troopers)
    {
        t->timeSinceBuy += gsio->window.i_deltaTime;
        t->Update(gsio, this);
    }

    for (MC_Enemy* t : enemies)
    {
        if (t->position < -2.0f)
        {
            dead = true;
            break;
        }
        t->Update(gsio, this);
    }

    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(), [](const MC_Enemy* obj) {
            if (obj->die)
            {
                delete obj;
                return true;
            }
            return false;
        }),
        enemies.end()
    );

    troopers.erase(
        std::remove_if(troopers.begin(), troopers.end(), [](const MC_Trooper* obj) {
            if (obj->die)
            {
                delete obj;
                return true;
            }
            return false;
        }),
        troopers.end()
    );

    if (enemies.size() == 0 && timeSinceWave > 1.0f)
    {
        wavePause = true;
    }

    if (troopers.size() == 0 && money < 150)
    {
        dead = true;
    }
}

void MC_Enemy::PlayRandomS(MC_Game* game, GSIO* gsio)
{
    
}

void MC_Enemy::Load(GSIO *gsio, MC_Game *game)
{
    mesh = new MC_EasyRender();
    mesh->Load(gsio, "EnemySh", "Enemy");
    xposition = (float)rand() / (float)RAND_MAX;
    float range = 1.0f;
    xposition *= range;
    xposition -= range/2.0f;

    position = 2.0f;

    gene = (((float)rand() / (float)RAND_MAX)*0.5f)+0.1f;

    health = (gene*10.0f*game->ProgCurve(game->waveIndex))+0.4f;

    mesh->model->transform.size = glm::vec3(gene);

    mesh->model->transform.position.x = xposition;
    mesh->model->transform.position.z = position;
}

void MC_Enemy::Update(GSIO *gsio, MC_Game *game)
{
    position -= gsio->window.i_deltaTime*0.2f*gene*(game->ProgCurve(game->waveIndex)/3.0f);
    mesh->model->transform.position.x = xposition;
    mesh->model->transform.position.z = position;

    if (health < 0.0f)
    {
        die = true;
        //player.LoadSoundPCM(&gsio->state->soundtrack.musicHost, &deathClip);
        //player.Play();
    }

    if (xposition < -2.0f)
    {
        game->dead = true;
    }
}

void MC_Trooper::TextSet(GSIO *gsio)
{
    sellPrice = timeSinceBuy*std::max(1.0f, gsio->state->game.ProgCurve(gsio->state->game.waveIndex)*(1+gsio->state->game.permaBonusCount));
    stats->transform.position.x = ((position/2.0f)*(gsio->window.w_width/2))+(gsio->window.w_width/2);
    stats->text = "Level "+std::to_string(level)+" Preis: "+std::to_string(sellPrice);
}

void MC_Trooper::TextInit(GSIO *gsio)
{
    stats = new MC_RenderedText();
    stats->Init(&gsio->engine_font, gsio);
    stats->text = "Test stat";
    stats->transform.rotation = glm::vec3(0.0f,0.0f,82.0f);
    stats->transform.position.y = gsio->window.w_height*0.2f;
    stats->transform.size = glm::vec3(0.3f);
    TextSet(gsio);
}

void MC_Trooper::TextUpdate(GSIO *gsio)
{
    TextSet(gsio);
}

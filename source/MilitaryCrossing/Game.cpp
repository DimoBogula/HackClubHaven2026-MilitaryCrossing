#include <MilitaryCrossing/Game.h>
#include <MilitaryCrossing/Render.h>
#include <MilitaryCrossing/Soundtrack.h>
#include <GSIO/GSIO.h>
#include <GSIO/drawing.h>

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
    player.LoadSoundPCM(&game->ahost, &clip);

    TextInit(gsio);

    SetPos();
}

void MC_BarricadeSoldier::Update(GSIO *gsio, MC_Game *game)
{
    SetPos();
    TextUpdate(gsio);

    for (MC_Enemy* en : game->enemies)
    {
        if (en->position-position < 4.0f && timer > 0.4f)
        {
            en->health -= 1.0f;
            timer = 0.0f;
            player.Play();
        }
    }

    timer += gsio->window.i_deltaTime;
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

    if (enemies.size() > 0)
    {
        gsio->state->soundtrack.ActivateTrack(2);
    }
    else
    {
        gsio->state->soundtrack.DeactivateTrack(2);
    }
    if (enemies.size() > 5)
    {
        gsio->state->soundtrack.ActivateTrack(3);
    }
    else
    {
        gsio->state->soundtrack.DeactivateTrack(3);
    }
    if (enemies.size() > 10)
    {
        gsio->state->soundtrack.ActivateTrack(4);
    }
    else
    {
        gsio->state->soundtrack.DeactivateTrack(4);
    }
    if (enemies.size() > 15)
    {
        gsio->state->soundtrack.ActivateTrack(5);
    }
    else
    {
        gsio->state->soundtrack.DeactivateTrack(5);
    }
}

void MC_Game::Update(GSIO *gsio)
{
    timeSinceWave += gsio->window.i_deltaTime;
    if (glfwGetKey(gsio->window.r_window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        selectedTroopIndex = 0;
    }
    if (glfwGetKey(gsio->window.r_window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        selectedTroopIndex = -1;
    }

    if (glfwGetKey(gsio->window.r_window, GLFW_KEY_A) == GLFW_PRESS)
        left = true;
    else if (glfwGetKey(gsio->window.r_window, GLFW_KEY_A) == GLFW_RELEASE)
        left = false;

    if (glfwGetKey(gsio->window.r_window, GLFW_KEY_D) == GLFW_PRESS)
        right = true;
    else if (glfwGetKey(gsio->window.r_window, GLFW_KEY_D) == GLFW_RELEASE)
        right = false;
    if (selectedTroopIndex >= 0)
    {
        float dir = 0.0f;
        if (left) dir -= 1.0f;
        if (right) dir += 1.0f;
        troopers[selectedTroopIndex]->position += dir*0.6f*gsio->window.i_deltaTime;
    }
    for (MC_Trooper* t : troopers)
    {
        t->timeSinceBuy += gsio->window.i_deltaTime;
        t->Update(gsio, this);
    }

    for (MC_Enemy* t : enemies)
    {
        t->Update(gsio, this);
    }
}

void MC_Enemy::PlayRandomS(MC_Game* game)
{
    player.LoadSoundPCM(&game->ahost, &clips[rand() % 4]);
    player.Play();
}

void MC_Enemy::Load(GSIO *gsio, MC_Game *game)
{
    mesh = new MC_EasyRender();
    mesh->Load(gsio, "EnemySh", "Enemy");
    xposition = (float)rand() / (float)RAND_MAX;
    float range = 1.0f;
    xposition *= range;
    xposition -= range/2.0f;

    for (int i = 0; i < 4; i++)
    {
        GSIO_audioClip& ac = clips.emplace_back();
        ac.Load(gsio->assetmanager.GetSoundAsset(std::string("a")+std::to_string(i)));
    }

    deathClip.Load(gsio->assetmanager.GetSoundAsset("endeath"));

    position = 2.0f;

    gene = (((float)rand() / (float)RAND_MAX)*0.5f)+0.1f;

    health = gene*10.0f*game->ProgCurve(game->waveIndex);

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
        player.LoadSoundPCM(&game->ahost, &deathClip);
    }
}

void MC_Trooper::TextSet(GSIO *gsio)
{
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

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

    SetPos();
}

void MC_BarricadeSoldier::Update(GSIO *gsio, MC_Game *game)
{
    SetPos();
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
        t->Update(gsio, this);
    }

    for (MC_Enemy* t : enemies)
    {
        t->Update(gsio, this);
    }
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

    mesh->model->transform.size = glm::vec3(gene);

    mesh->model->transform.position.x = xposition;
    mesh->model->transform.position.z = position;
}

void MC_Enemy::Update(GSIO *gsio, MC_Game *game)
{
    position -= gsio->window.i_deltaTime*0.2f;
    mesh->model->transform.position.x = xposition;
    mesh->model->transform.position.z = position;
}

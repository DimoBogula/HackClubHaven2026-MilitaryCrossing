#include "Game.h"
#include "Render.h"
#include <GSIO/GSIO.h>
#include <GSIO/drawing.h>

void MC_BarricadeSoldier::SetPos()
{
    meshes[0]->model->transform.position.x = position-0.6f;
    meshes[1]->model->transform.position.x = position;
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

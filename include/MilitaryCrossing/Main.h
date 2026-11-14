#pragma once
#include "Soundtrack.h"
#include "Render.h"

class GSIO;

class MC_State
{
    public:
    float time = 0.0f;
    MC_Soundtrack soundtrack;
    MC_Renderer renderer;
};

void MC_Start(GSIO* gsio);

void MC_Update(GSIO* gsio);
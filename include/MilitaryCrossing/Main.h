#pragma once
#include "Soundtrack.h"

class GSIO;

class MC_State
{
    public:
    MC_Soundtrack soundtrack;
};

void MC_Start(GSIO* gsio);

void MC_Update(GSIO* gsio);
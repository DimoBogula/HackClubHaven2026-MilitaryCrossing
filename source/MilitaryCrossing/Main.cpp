#include <MilitaryCrossing/Main.h>
#include <GSIO/GSIO.h>

void MC_Start(GSIO *gsio)
{
    gsio->state = new MC_State();
    gsio->state->soundtrack.Load(gsio);
    gsio->state->soundtrack.Test();
}

void MC_Update(GSIO *gsio)
{
    
}

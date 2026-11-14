#include <MilitaryCrossing/Main.h>
#include <GSIO/GSIO.h>

void MC_Start(GSIO *gsio)
{
    gsio->window.d_text.text = "";
    gsio->state = new MC_State();
    gsio->state->soundtrack.Load(gsio);
    gsio->state->soundtrack.ActivateTrack(0);
    gsio->state->soundtrack.ActivateTrack(1);

    gsio->state->renderer.Load(gsio);
}

void MC_Update(GSIO *gsio)
{
    gsio->state->time += gsio->window.i_deltaTime;
    gsio->state->soundtrack.Update(gsio);
    gsio->state->renderer.Update(gsio);
}

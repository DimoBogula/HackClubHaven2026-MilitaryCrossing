#include <MilitaryCrossing/Main.h>
#include <GSIO/GSIO.h>

void MC_Start(GSIO *gsio)
{
    gsio->window.d_text.text = "Military Crossing";
    gsio->state = new MC_State();
    gsio->state->soundtrack.Load(gsio);
    gsio->state->soundtrack.ActivateTrack(0);
    gsio->state->soundtrack.ActivateTrack(1);
}

void MC_Update(GSIO *gsio)
{
    gsio->state->soundtrack.Update(gsio);
    gsio->window.d_text.text = std::to_string(gsio->state->soundtrack.musicConfig[0].timeSince);
}

#pragma once
#define GSIO_APPID 480
#define GSIO_APPNAME "Geese Oil Corp"
#define GSIO_Version_String "0.0.1"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <GL/glm/glm.hpp>
#include <steam/steam_api.h>
#include "response.h"
#include "window.h"
#include "assetmanager.h"
#include "text.h"
#include "drawing.h"

#include <MilitaryCrossing/Main.h>

class GSIO;

GSIO_response GSIO_Init(GSIO* gsio);
GSIO_response GSIO_Shutdown(GSIO* gsio);
GSIO_response GSIO_Application(GSIO* gsio);
GSIO_response GSIO_Heartbeat(GSIO* gsio);

class GSIO
{
    public:
    FT_Library ft;
    GSIO_textfont engine_font;
    GSIO_material engine_font_material;
    GSIO_window window;
    GSIO_assetmanager assetmanager;

    MC_State* state = nullptr;

    GSIO() {};
};
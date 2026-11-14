#pragma once
#include <string>
#include <cstdint>
#include "text.h"

class GLFWwindow;
class GSIO_textureasset;

class GSIO_window
{
    public:
    GLFWwindow* r_window = nullptr;
    uint32_t w_width = 0, w_height = 0;
    double i_deltaTime = 0.0f;
    float i_pastTime = 0.0f;
    std::string w_name;
    GSIO_text d_text;

    void Render(GSIO* gsio);
    void PreRender(GSIO* gsio);
    void SetIcon(GSIO_textureasset* texture, GSIO_textureasset* textureSmall);

    GSIO_window(uint32_t w, uint32_t h, std::string n);
    GSIO_window() {}
};
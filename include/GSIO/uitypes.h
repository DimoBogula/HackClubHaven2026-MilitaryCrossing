#pragma once

class GSIO_color
{
    public:
    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;
    float a = 0.0f;
};

class GSIO_imageData
{
    public:
    std::vector<unsigned char> image;
    unsigned width = 0, height = 0;
};
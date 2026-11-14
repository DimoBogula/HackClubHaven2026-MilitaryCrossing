#include <GSIO/texturesheet.h>
#include <rect_pack.h>
#include <lodepng.h>
#include <fstream>
#include <sstream>
#include <GSIO/GSIO.h>
#include <filesystem>
#include <iostream>

GSIO_response GSIO_texturesheet::LoadData(GSIO_assetdatastructure assetData)
{
    /*if (assetData.tryLoadFromCache)
    {
        std::string cachePath = GSIO_texture_cache_folder+assetData.name+GSIO_texture_cache_ext;
        if (std::filesystem::exists(GSIO_texture_cache_folder+assetData.name+GSIO_texture_cache_ext))
        {
            std::ifstream inFile(cachePath, std::ios::binary | std::ios::ate);
            if (!inFile.is_open()) {
                return {false, GSIO_response_Error_Prefix " - Error opening cache file."};
            }

            std::streamsize fileSize = inFile.tellg();
            if (fileSize <= static_cast<std::streamsize>(sizeof(decltype(image.width))*2)) {
                return {false, GSIO_response_Error_Prefix " - Invalid texture cache."};
            }

            std::streamsize imageSize = fileSize - sizeof(decltype(image.width))*2;
            image.image.resize(imageSize);

            inFile.seekg(0, std::ios::beg);
            inFile.read(reinterpret_cast<char*>(&image.width), sizeof(decltype(image.width)));
            inFile.read(reinterpret_cast<char*>(&image.height), sizeof(decltype(image.height)));
            inFile.read(reinterpret_cast<char*>(image.image.data()), sizeof(unsigned char)*imageSize);
            return {true};
        }
    }*/

    std::vector<std::string> files;

    if (assetData.ini.HasContent("Texturesheet") == false) return {false, GSIO_response_Error_Prefix " - No Texturesheet section."};

    for (int i = 0; i < assetData.ini.GetContent("Texturesheet").variables.size(); i++)
    {
        files.push_back(std::filesystem::path(assetData.fullPath).parent_path().string()+"/"+assetData.ini.GetContent("Texturesheet").variables[i].name);
    }

    std::vector<GSIO_imageData> images;
    images.resize(files.size());

    for (int i = 0; i < files.size(); i++)
    {
        unsigned error = lodepng::decode(images[i].image, images[i].width, images[i].height, files[i]);

        if(error) return {false, GSIO_response_Error_Prefix " - Image decoder error "+std::to_string(error)+": "+std::string(lodepng_error_text(error))};   
    }

    std::vector<rect_pack::Size> sizes;

    for (int i = 0; i < images.size(); i++)
    {
        rect_pack::Size size;
        size.height = images[i].height;
        size.width = images[i].width;
        size.id = i;
        sizes.push_back(size);
    }

    rect_pack::Settings settings;
    settings.allow_rotate = false;
    settings.max_sheets = 1;
    settings.method = rect_pack::Method::Best;
    settings.min_width = 0;
    settings.min_height = 0;
    settings.max_width = 4096;
    settings.max_height = 4096;
    settings.power_of_two = false;
    settings.square = false;
    settings.align_width = 0;
    settings.border_padding = 0;
    settings.over_allocate = 0;

    std::vector<rect_pack::Sheet> sheets = rect_pack::pack(settings, sizes);
    if (sheets.size() < 1) return {false, GSIO_response_Error_Prefix " - No sheets generated."};
    image.width = sheets[0].width;
    image.height = sheets[0].height;
    image.image.resize(sheets[0].width*sheets[0].height*4);
    for (int x = 0; x < sheets[0].width; x++)
    {
        for (int y = 0; y < sheets[0].height; y++)
        {
            bool intersected = false;
            int base_index = (y * sheets[0].width + x) * 4;
            for (int i = 0; i < sheets[0].rects.size(); i++)
            {
                rect_pack::Rect& rect = sheets[0].rects[i];
                bool intersect = (x >= rect.x) && (x < rect.x + rect.width) && (y >= rect.y) && (y < rect.y + rect.height);
                if (intersect)
                {
                    intersected = true;
                    int transX = x-rect.x;
                    int transY = y-rect.y;
                    int target_base_index = (transY * rect.width + transX) * 4;
                    for (int j = 0; j < 4; j++)
                        image.image[base_index + j] = images[rect.id].image[target_base_index + j];
                    break;
                }
            }
            if (!intersected)
            {
                for (int j = 0; j < 4; j++)
                    image.image[base_index + j] = 0;
            }
        }
    }
    for (int i = 0; i < sheets[0].rects.size(); i++)
    {
        rect_pack::Rect& rect = sheets[0].rects[i];
        GSIO_texturesheetEntry& entry = entries.emplace_back();
        entry.fileName = assetData.ini.GetContent("Texturesheet").variables[i].name;
        entry.width = rect.width;
        entry.height = rect.height;
        entry.offset = glm::vec2(rect.x, rect.y);
        entry.uv = glm::vec4((float)rect.x/(float)image.width, ((float)rect.y+(float)rect.height)/(float)image.height, ((float)rect.x+(float)rect.width)/(float)image.width, (float)rect.y/(float)image.height);
    }
    return {true};
}

GSIO_response GSIO_texturesheet::CacheData(GSIO_assetdatastructure assetData)
{
    if (!assetData.cache) return {true};
    /*std::string cachePath = GSIO_texture_cache_folder+assetData.name+GSIO_texture_cache_ext;
    std::filesystem::create_directories(GSIO_texture_cache_folder);

    std::ofstream TextureCacheFile(cachePath, std::ios::binary | std::ios::trunc);
    if (!TextureCacheFile.is_open())
    {
        return {false, GSIO_response_Warning_Prefix "Unable to open texture cache file."};
    }
	TextureCacheFile.write(reinterpret_cast<const char*>(&image.width), sizeof(decltype(image.width)));
    TextureCacheFile.write(reinterpret_cast<const char*>(&image.height), sizeof(decltype(image.height)));
    TextureCacheFile.write(reinterpret_cast<const char*>(image.image.data()), image.image.size()*sizeof(unsigned char));
	TextureCacheFile.close();*/
    return {true};
}

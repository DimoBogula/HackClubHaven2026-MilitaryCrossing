#include <GSIO/textureasset.h>
#include <lodepng.h>
#include <fstream>
#include <sstream>
#include <GSIO/GSIO.h>
#include <filesystem>

GSIO_response GSIO_textureasset::LoadData(GSIO_assetdatastructure assetData)
{
    if (assetData.tryLoadFromCache)
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
    }

    unsigned error = lodepng::decode(image.image, image.width, image.height, assetData.fullPath+".png");

    if(error) return {false, GSIO_response_Error_Prefix " - Image decoder error "+std::to_string(error)+": "+std::string(lodepng_error_text(error))};
    return {true};
}

GSIO_response GSIO_textureasset::CacheData(GSIO_assetdatastructure assetData)
{
    if (!assetData.cache) return {true};
    std::string cachePath = GSIO_texture_cache_folder+assetData.name+GSIO_texture_cache_ext;
    std::filesystem::create_directories(GSIO_texture_cache_folder);

    std::ofstream TextureCacheFile(cachePath, std::ios::binary | std::ios::trunc);
    if (!TextureCacheFile.is_open())
    {
        return {false, GSIO_response_Warning_Prefix "Unable to open texture cache file."};
    }
	TextureCacheFile.write(reinterpret_cast<const char*>(&image.width), sizeof(decltype(image.width)));
    TextureCacheFile.write(reinterpret_cast<const char*>(&image.height), sizeof(decltype(image.height)));
    TextureCacheFile.write(reinterpret_cast<const char*>(image.image.data()), image.image.size()*sizeof(unsigned char));
	TextureCacheFile.close();
    return {true};
}

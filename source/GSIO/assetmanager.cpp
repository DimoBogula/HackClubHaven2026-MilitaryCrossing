#include <GSIO/assetmanager.h>
#include <GSIO/shader.h>
#include <GSIO/sound.h>
#include <GSIO/mesh.h>
#include <GSIO/textureasset.h>
#include <GSIO/texturesheet.h>
#include <filesystem>
#include <iostream>

GSIO_response GSIO_Init_assetmanager(GSIO_assetmanager* assm)
{
    for (GSIO_asset& asset : assm->assets)
    {
        std::cout << "Loading... " << asset.assetData.name << std::flush;
        GSIO_response r = assm->InitAsset(asset);
        std::cout << " loaded!" << std::endl;
        if (r.success == false) return r;
    }
    return {true};
}

GSIO_response GSIO_assetmanager::LoadAllAssets()
{
    std::vector<std::string> files;

    for (std::filesystem::recursive_directory_iterator it(GSIO_assetmanager_Default_Path_Assets), end; it != end; ++it) {
        if (!is_directory(it->path())) {
            std::string ps = it->path().string();
            std::string en = GSIO_assetmanager_Default_File_Ext;
            if (!ps.compare(ps.size()-en.size(), en.size(), en))
            {
                files.push_back(ps);
            }
        }
    }

    if (files.size() + 1 > assets.capacity()) assets.reserve(files.size());

    for (std::string const& filePath : files) {
        std::cout << "Registering... " << filePath;
        if (AssetExists_Path(filePath))
        {
            std::cout << " already registered!" << std::endl;
            continue;
        }
        GSIO_asset& asset = assets.emplace_back();
        asset.assetData = GSIO_assetdatastructure(filePath);
        std::cout << " registered!" << std::endl;
    }
    GSIO_response r = GSIO_Init_assetmanager(this);
    return r;
}

GSIO_response GSIO_assetmanager::CacheAllAssets()
{
    for (GSIO_asset& asset : assets)
    {
        if (asset.assetData.cache)
        {
            GSIO_response r = asset.data->CacheData(asset.assetData);
            if (r.success == false) return r;
        }
    }
    return {true};
}

GSIO_response GSIO_assetmanager::LoadAsset(std::string path)
{
    GSIO_asset& asset = assets.emplace_back();
    asset.assetData = GSIO_assetdatastructure(GSIO_assetmanager_Default_Path_Assets+path+".ini");
    return InitAsset(asset);
}

GSIO_response GSIO_assetmanager::InitAsset(GSIO_asset &asset)
{
    if (asset.loaded) return {true};
    asset.loaded = true;
    switch (asset.assetData.type)
    {
        case GSIO_assettype::Shader:
        asset.data = new GSIO_shaderasset();
        break;
        case GSIO_assettype::Sound:
        asset.data = new GSIO_soundasset();
        break;
        case GSIO_assettype::Model:
        asset.data = new GSIO_meshasset();
        break;
        case GSIO_assettype::Texture:
        asset.data = new GSIO_textureasset();
        break;
        case GSIO_assettype::Texturesheet:
        asset.data = new GSIO_texturesheet();
        break;
        default:
        asset.data = new GSIO_assetdata();
        break;
    }
    return asset.data->LoadData(asset.assetData);
}

bool GSIO_assetmanager::AssetExists_Path(std::string path)
{
    for (GSIO_asset& asset : assets)
    {
        if (asset.assetData.fullPath+".ini" == path)
        {
            return true;
        }
    }
    return false;
}

GSIO_asset *GSIO_assetmanager::GetAsset(std::string name)
{
    for (GSIO_asset& asset : assets)
    {
        if (asset.assetData.name == name) return &asset;
    }
    return nullptr;
}

GSIO_shaderasset *GSIO_assetmanager::GetShaderAsset(std::string name)
{
    GSIO_asset* pa = GetAsset(name);
    if (pa != nullptr) return static_cast<GSIO_shaderasset*>(pa->data);
    return nullptr;
}

GSIO_soundasset *GSIO_assetmanager::GetSoundAsset(std::string name)
{
    GSIO_asset* pa = GetAsset(name);
    if (pa != nullptr) return static_cast<GSIO_soundasset*>(pa->data);
    return nullptr;
}

GSIO_meshasset *GSIO_assetmanager::GetMeshAsset(std::string name)
{
    GSIO_asset* pa = GetAsset(name);
    if (pa != nullptr) return static_cast<GSIO_meshasset*>(pa->data);
    return nullptr;
}

GSIO_textureasset *GSIO_assetmanager::GetTextureAsset(std::string name)
{
    GSIO_asset* pa = GetAsset(name);
    if (pa != nullptr) return static_cast<GSIO_textureasset*>(pa->data);
    return nullptr;
}

GSIO_texturesheet *GSIO_assetmanager::GetTexturesheetAsset(std::string name)
{
    GSIO_asset* pa = GetAsset(name);
    if (pa != nullptr) return static_cast<GSIO_texturesheet*>(pa->data);
    return nullptr;
}

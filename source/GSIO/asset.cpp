#include <GSIO/assetmanager.h>
#include <ini.h>
#include <fstream>
#include <sstream>
#include <magic_enum/magic_enum.hpp>
#include <filesystem>

GSIO_response GSIO_assetdata::LoadData(GSIO_assetdatastructure assetData)
{
    return {true, GSIO_response_Warning_Prefix " - No data loaded."};
}

GSIO_response GSIO_assetdata::CacheData(GSIO_assetdatastructure assetData)
{
    return {true, GSIO_response_Info_Prefix " - No data cached."};
}

GSIO_asset::~GSIO_asset()
{
    if (data != nullptr) delete data;
    data = nullptr;
}

GSIO_assetdatastructure::GSIO_assetdatastructure(std::string filePath)
{
    filePath = filePath;
    std::string iniContent;
    std::ifstream iniFile;
    iniFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    iniFile.open(filePath);
    std::stringstream iniFileStream;
    iniFileStream << iniFile.rdbuf();
    iniFile.close();
    iniContent = iniFileStream.str();
    ini.Parse(iniContent);

    if (ini.HasContent("Data"))
    {
        if (ini.GetContent("Data").HasVariable("type"))
        {
            auto v_type = magic_enum::enum_cast<GSIO_assettype>(ini.GetContent("Data").GetVariable("type").value, magic_enum::case_insensitive);
            type = v_type.value_or(GSIO_assettype::Unknown);
        }
        if (ini.GetContent("Data").HasVariable("name"))
        {
            name = ini.GetContent("Data").GetVariable("name").value;
        }
        if (ini.GetContent("Data").HasVariable("cache"))
        {
            cache = ini.GetContent("Data").GetVariable("cache").value == "true";
        }
    }

    fullPath = std::filesystem::path(filePath).parent_path().string()+"/"+name;
}

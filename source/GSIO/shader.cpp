#include <GSIO/shader.h>
#include <fstream>
#include <sstream>
#include <GSIO/GSIO.h>
#include <filesystem>

void GSIO_shaderasset::use()
{
    glUseProgram(ID);
}

GSIO_response GSIO_shaderasset::CheckCompileErrors(GLuint shader, std::string type)
{
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            return {false, GSIO_response_Error_Prefix " - Shader compilation failed " + type + infoLog};
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            return {false, GSIO_response_Error_Prefix " - Shader linking failed " + type + infoLog};
        }
    }
    return {true};
}

GSIO_response GSIO_shaderasset::LoadShaderFromCache(GSIO_assetdatastructure data)
{
    std::string cacheFolder = GSIO_shader_cache_folder;
    std::string cachePath = cacheFolder+data.name+GSIO_shader_cache_ext;
    if (!std::filesystem::exists(cachePath))
    {
        return {false, GSIO_response_Error_Prefix " - Cache file doesn't exist."};
    }

    std::ifstream inFile(cachePath, std::ios::binary | std::ios::ate);
    if (!inFile.is_open()) {
        return {false, GSIO_response_Error_Prefix " - Error opening cache file."};
    }

    std::streamsize fileSize = inFile.tellg();
    if (fileSize <= static_cast<std::streamsize>(sizeof(GLenum))) {
        return {false, GSIO_response_Error_Prefix " - Invalid shader program."};
    }
    std::streamsize binarySize = fileSize - sizeof(GLenum);

    inFile.seekg(0, std::ios::beg);

    GLenum binaryFormat = 0;
    inFile.read(reinterpret_cast<char*>(&binaryFormat), sizeof(GLenum));

    std::vector<GLubyte> binary(binarySize);
    inFile.read(reinterpret_cast<char*>(binary.data()), binarySize);

    GLuint program = glCreateProgram();
    glProgramBinary(program, binaryFormat, binary.data(), static_cast<GLsizei>(binarySize));

    GLint success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        GLint logLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
        
        std::string errorLog = "Unknown error";
        if (logLength > 1) {
            std::vector<char> infoLog(logLength);
            GLsizei actualLength = 0;
            glGetProgramInfoLog(program, logLength, &actualLength, infoLog.data());
            if (actualLength > 0) {
                errorLog = std::string(infoLog.data(), actualLength);
            }
        }
        
        glDeleteProgram(program);
        return {false, GSIO_response_Error_Prefix " - Failed to load shader program. | " + errorLog};
    }
    ID = program;
    return {true};
}

GSIO_response GSIO_shaderasset::LoadShader(GSIO_assetdatastructure& assetData, unsigned int* ver, unsigned int* fra, unsigned int* geo, unsigned int* pro)
{
    std::string path = assetData.fullPath;
    std::string vertexPath = path+".vs";
    std::string fragmentPath = path+".fs";
    std::string computePath = path+".cs";
    std::string geometryPath = path+".gs";
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    std::string computeCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    std::ifstream gShaderFile;
    std::ifstream cShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    gShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    cShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try 
    {
        if(geometryShader)
        {
            gShaderFile.open(geometryPath);
            std::stringstream ShaderStream;
            ShaderStream << gShaderFile.rdbuf();
            gShaderFile.close();
            geometryCode = ShaderStream.str();
        }
        if(computeShader)
        {
            cShaderFile.open(computePath);
            std::stringstream ShaderStream;
            ShaderStream << cShaderFile.rdbuf();
            cShaderFile.close();
            computeCode = ShaderStream.str();
        }
        if(vertexShader)
        {
            vShaderFile.open(vertexPath);
            std::stringstream ShaderStream;
            ShaderStream << vShaderFile.rdbuf();
            vShaderFile.close();
            vertexCode = ShaderStream.str();
        }
        if(fragmentShader)
        {
            fShaderFile.open(fragmentPath);
            std::stringstream ShaderStream;
            ShaderStream << fShaderFile.rdbuf();
            fShaderFile.close();
            fragmentCode = ShaderStream.str();
        }
    }
    catch (std::ifstream::failure& e)
    {
        return {false, GSIO_response_Error_Prefix " - File not successfuly read while compiling a shader, path: "+path};
    }
    unsigned int geometry, compute, vertex, fragment;
    if (geometryShader)
    {
        const char * ShaderCode = geometryCode.c_str();
        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry, 1, &ShaderCode, NULL);
        glCompileShader(geometry);
        GSIO_response lastReponse = CheckCompileErrors(geometry, "COMPUTE");
        if (!lastReponse.success) return lastReponse;
    }
    if (computeShader)
    {
        const char * ShaderCode = computeCode.c_str();
        compute = glCreateShader(GL_COMPUTE_SHADER);
        glShaderSource(compute, 1, &ShaderCode, NULL);
        glCompileShader(compute);
        GSIO_response lastReponse = CheckCompileErrors(compute, "COMPUTE");
        if (!lastReponse.success) return lastReponse;
    }
    if (vertexShader)
    {
        const char * ShaderCode = vertexCode.c_str();
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &ShaderCode, NULL);
        glCompileShader(vertex);
        GSIO_response lastReponse = CheckCompileErrors(vertex, "VERTEX");
        if (!lastReponse.success) return lastReponse;
    }
    if (fragmentShader)
    {
        const char * ShaderCode = fragmentCode.c_str();
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &ShaderCode, NULL);
        glCompileShader(fragment);
        GSIO_response lastReponse = CheckCompileErrors(fragment, "FRAGMENT");
        if (!lastReponse.success) return lastReponse;
    }
    // shader Program
    ID = glCreateProgram();
    if(vertexShader) glAttachShader(ID, vertex);
    if(fragmentShader) glAttachShader(ID, fragment);
    if(geometryShader) glAttachShader(ID, geometry);
    if(computeShader) glAttachShader(ID, compute);
    glLinkProgram(ID);
    GSIO_response lastReponse = CheckCompileErrors(ID, "PROGRAM");
    if (!lastReponse.success) return lastReponse;
    if(geometryShader) glDeleteShader(geometry);
    if(fragmentShader) glDeleteShader(fragment);
    if(vertexShader) glDeleteShader(vertex);
    if(computeShader) glDeleteShader(compute);
    return {true};
}

GSIO_response GSIO_shaderasset::LoadData(GSIO_assetdatastructure data)
{
    if (data.ini.HasContent("Shader"))
    {
        if (data.ini.GetContent("Shader").HasVariable("geometryShader"))
        {
            geometryShader = data.ini.GetContent("Shader").GetVariable("geometryShader").value == "true";
        }
        if (data.ini.GetContent("Shader").HasVariable("computeShader"))
        {
            computeShader = data.ini.GetContent("Shader").GetVariable("computeShader").value == "true";
        }
        if (data.ini.GetContent("Shader").HasVariable("vertexShader"))
        {
            vertexShader = data.ini.GetContent("Shader").GetVariable("vertexShader").value == "true";
        }
        if (data.ini.GetContent("Shader").HasVariable("fragmentShader"))
        {
            fragmentShader = data.ini.GetContent("Shader").GetVariable("fragmentShader").value == "true";
        }
    }
    if (data.tryLoadFromCache && data.cache)
    {
        GSIO_response tryCache = LoadShaderFromCache(data);
        if (tryCache.success) return {true};
    }
    unsigned int vertex, fragment, geometry, program;
    return LoadShader(data, &vertex, &fragment, &geometry, &program);
}

GSIO_response GSIO_shaderasset::CacheData(GSIO_assetdatastructure data)
{
    if (ID == 0) return {false, GSIO_response_Warning_Prefix "No shader program provided. | "+data.name};

    GLint binaryLength = 0;
    glGetProgramiv(ID, GL_PROGRAM_BINARY_LENGTH, &binaryLength);

    std::vector<GLubyte> binaryBuffer(binaryLength);
    GLenum binaryFormat = 0;
    GLsizei actualLength = 0;
    glGetProgramBinary(ID, binaryLength, &actualLength, &binaryFormat, binaryBuffer.data());
    std::string cacheFolder = GSIO_shader_cache_folder;
    std::string cachePath = cacheFolder+data.name+GSIO_shader_cache_ext;
    std::filesystem::create_directories(cacheFolder);

    std::ofstream ShaderCacheFile(cachePath, std::ios::binary | std::ios::trunc);
    if (!ShaderCacheFile.is_open())
    {
        return {false, GSIO_response_Warning_Prefix "Unable to open shader cache file."};
    }
	ShaderCacheFile.write(reinterpret_cast<const char*>(&binaryFormat), sizeof(GLenum));
    ShaderCacheFile.write(reinterpret_cast<const char*>(binaryBuffer.data()), actualLength);
	ShaderCacheFile.close();
    return {true};
}

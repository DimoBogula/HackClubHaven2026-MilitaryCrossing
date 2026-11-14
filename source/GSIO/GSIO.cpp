#include <GSIO/GSIO.h>
#include <MilitaryCrossing/Main.h>
#include <iostream>

#define GSIO_DISABLE_STEAM

void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam)
{
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return; 

    std::cout << "opengl error " << id << "\n";
    std::cout << "Message: " << message << "\n";

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
    } std::cout << "\n";

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
        case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
        case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
    } std::cout << "\n";

    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: HIGH"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: MEDIUM"; break;
        case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: LOW"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: NOTIFICATION"; break;
    } std::cout << "\n" << std::endl << std::flush;
}

GSIO_response GSIO_Init(GSIO* gsio)
{
    #ifndef GSIO_DISABLE_STEAM
    if ( SteamAPI_RestartAppIfNecessary( GSIO_APPID ) )
	{
		return {false, GSIO_response_Error_Prefix " - The app must restart, as it is necessary."};
	}

	if ( !SteamAPI_Init() )
	{
		return {false, GSIO_response_Error_Prefix " - Steam must be running to play this game (SteamAPI_Init() failed)."};
	}
    #endif

    if (!glfwInit()) return {false, GSIO_response_Error_Prefix " - glfwInit() failed."};

    if (FT_Init_FreeType(&gsio->ft)) {
        return {false, GSIO_response_Error_Prefix " - FT_Init_FreeType failed."};
    }

    gsio->window = GSIO_window(1280, 720, GSIO_APPNAME);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        return {false, GSIO_response_Error_Prefix " - gladLoadGLLoader() failed."};
    }

    GLint flags; 
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }

    gsio->assetmanager.assets.reserve(30);

    GSIO_response loadEngineFont = GSIO_Load_Font(&gsio->engine_font, gsio, std::string(GSIO_assetmanager_Default_Path_Assets "engine_font"));

    if (!loadEngineFont.success)
    {
        return {false, GSIO_response_Error_Prefix " - Failed to load engine font. | "+loadEngineFont.message};
    }

    GSIO_response loadEngineFontMaterial = gsio->assetmanager.LoadAsset("shaders/engine/engine_debugtext");

    if (!loadEngineFontMaterial.success)
    {
        return {false, GSIO_response_Error_Prefix " - Failed to load engine font shader. | "+loadEngineFontMaterial.message};
    }

    gsio->engine_font_material.shaderID = gsio->assetmanager.GetShaderAsset("engine_debugtext")->ID;

    gsio->window.d_text = GSIO_text(true);
    gsio->window.d_text.visible = true;
    gsio->window.d_text.text = "Loading - Assets";
    gsio->window.PreRender(gsio);
    gsio->window.Render(gsio);
    GSIO_response loadThings = gsio->assetmanager.LoadAllAssets();
    if (!loadThings.success)
    {
        return {false, GSIO_response_Error_Prefix " - Failed to load assets. | "+loadThings.message};
    }
    gsio->window.d_text.text = "Caching - Assets";
    gsio->window.PreRender(gsio);
    gsio->window.Render(gsio);
    GSIO_response cacheThings = gsio->assetmanager.CacheAllAssets();
    if (!cacheThings.success)
    {
        return {false, GSIO_response_Error_Prefix " - Failed to cache assets. | "+cacheThings.message};
    }
    gsio->window.d_text.text = "Loading & Caching - Finished";
    gsio->window.PreRender(gsio);
    gsio->window.Render(gsio);

    return {true};
}

GSIO_response GSIO_Shutdown(GSIO* gsio)
{
    FT_Done_FreeType(gsio->ft);
    #ifndef GSIO_DISABLE_STEAM
    SteamAPI_Shutdown();
    #endif
    glfwDestroyWindow(gsio->window.r_window);
    glfwTerminate();
    return {true};
}

GSIO_response GSIO_Application(GSIO *gsio)
{
    if (gsio->window.r_window == nullptr) return {false, GSIO_response_Error_Prefix " - window is nullptr."};
    GSIO_response heartbeat = GSIO_Heartbeat(gsio);
    MC_Start(gsio);
    while (heartbeat.success)
    {
        #ifndef GSIO_DISABLE_STEAM
        SteamAPI_RunCallbacks();
        #endif

        gsio->window.PreRender(gsio);
        MC_Update(gsio);
        gsio->window.Render(gsio);

        glfwPollEvents();
        heartbeat = GSIO_Heartbeat(gsio);
    }
    heartbeat.success = true;
    return heartbeat;
}

GSIO_response GSIO_Heartbeat(GSIO *gsio)
{
    if (glfwWindowShouldClose(gsio->window.r_window)) return {false, GSIO_response_Info_Prefix " - game is closing due to user close."};
    return {true};
}

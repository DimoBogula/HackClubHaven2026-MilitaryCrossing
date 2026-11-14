#ifdef COMPILE_WHAT_SHOULDNT_BE
#include <GeeseOilCorp/GOCState.h>
#include <GSIO/GSIO.h>
#include <GSIO/mesh.h>
#include <GSIO/drawing.h>
#include <GSIO/textureasset.h>
#include <GSIO/texturesheet.h>
#include <GL/glm/glm.hpp>
#include <GL/glm/gtc/matrix_transform.hpp>
#include <GL/glm/gtc/type_ptr.hpp>

void GeeseOilCorpGlobalState::Init(GSIO *gsio)
{
    GSIO_textureasset* icon = gsio->assetmanager.GetTextureAsset("Icon");
    gsio->window.SetIcon(icon, icon);
    playerView = new GSIO_capturer();
    playerRenderer = new GSIO_renderer();
    playerRenderer->shaders.push_back(gsio->assetmanager.GetShaderAsset("screenRender"));
    playerRenderer->Init(gsio);
    playerView->clearColor = {0.0f,0.0f,0.0f,1.0f};
    playerView->Init(gsio);

    solidMaterial.shaderID = gsio->assetmanager.GetShaderAsset("solid3D")->ID;
    monkeyUV.InitTexture(GSIO_textureFilter::Linear, GL_TEXTURE0, &gsio->assetmanager.GetTextureAsset("UVTest")->image);
    solidMaterial.textures.push_back(&monkeyUV);

    sheet = gsio->assetmanager.GetTexturesheetAsset("TestSheet");

    textureDisplay.position = glm::vec2(0.0f, 0.0f);
    textureDisplay.centered = true;
    textureDisplay.size = glm::vec2((float)sheet->entries[0].width, (float)sheet->entries[0].height);
    textureDisplay.uv = sheet->entries[0].uv;
    sheetTexture.InitTexture(GSIO_textureFilter::Pixel, GL_TEXTURE0, &sheet->image);

    textureMaterial.shaderID = gsio->assetmanager.GetShaderAsset("texture3DQuad")->ID;
    textureMaterial.textures.push_back(&sheetTexture);

    sampleArtist = new GSIO_artist();
    GSIO_meshasset* monkey = gsio->assetmanager.GetMeshAsset("Monkey");
    sampleArtist->mesh.vertices = monkey->vertices;
    sampleArtist->mesh.indices = monkey->indices;
    sampleArtist->mesh.GenerateMesh();
    sampleArtist->transform.position = glm::vec3(0.0f,0.0f,0.0f);
    sampleArtist->transform.size = glm::vec3(1.0f);
    sampleArtist->materials.push_back(solidMaterial);

    text = GSIO_text(true);
    text.text = "Test text";
    text.visible = true;

    /*audioClip.Load(gsio->assetmanager.GetSoundAsset("osc_135.50"));
    audioPlayer.LoadSoundPCM(&audioHost, &audioClip);
    audioPlayer.Play();*/
}

void GeeseOilCorpGlobalState::Update(GSIO *gsio)
{
    GSIO_renderContext renderContext;
    renderContext.depthTexture = &playerView->depthTexture;
    renderContext.captureTexture = &playerView->texture;
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);
    renderContext.view = glm::lookAt(cameraPos, cameraTarget, upVector);
    renderContext.projection = glm::perspective(glm::radians(80.0f), (float)gsio->window.w_width/(float)gsio->window.w_height, 0.1f,100.0f);

    playerView->Clear();

    sampleArtist->transform.rotation.y = frame;
    playerView->RenderArtist(renderContext, sampleArtist);
    GSIO_transform quadTransform;
    quadTransform.position = glm::vec3(0.0f);
    quadTransform.rotation = glm::vec3(0.0f);
    quadTransform.size = glm::vec3(0.01f);
    textureDisplay.centered = false;
    //playerView->RenderQuad(renderContext, &textureDisplay, &textureMaterial, &quadTransform);
    playerRenderer->Render(gsio, renderContext);

    frame++;
    gsio->window.d_text.text = "frame "+std::to_string(gsio->gocgs->frame)+" at fps "+std::to_string(1.0f/gsio->window.i_deltaTime);
}
#endif
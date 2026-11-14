#include <MilitaryCrossing/Render.h>
#include <MilitaryCrossing/Game.h>
#include <GSIO/drawing.h>
#include <GSIO/shader.h>
#include <GSIO/texture.h>
#include <GSIO/GSIO.h>
#include <GSIO/mesh.h>
#include <GSIO/textureasset.h>
#include <GL/glm/glm.hpp>
#include <GL/glm/gtc/matrix_transform.hpp>
#include <GL/glm/gtc/type_ptr.hpp>

void MC_Renderer::Load(GSIO *gsio)
{
    playerView = new GSIO_capturer();
    playerRenderer = new GSIO_renderer();
    playerRenderer->shaders.push_back(gsio->assetmanager.GetShaderAsset("screenRender"));
    playerRenderer->Init(gsio);
    playerView->clearColor = {0.0f,0.0f,0.0f,1.0f};
    playerView->Init(gsio);

    uiView = new GSIO_capturer();
    uiRenderer = new GSIO_renderer();
    uiRenderer->shaders.push_back(gsio->assetmanager.GetShaderAsset("uiscreenRender"));
    uiRenderer->Init(gsio);
    uiView->clearColor = {0.0f,0.0f,0.0f,1.0f};
    uiView->Init(gsio);

    streetMaterial.shaderID = gsio->assetmanager.GetShaderAsset("StreetShader")->ID;
    streetMaterial.SetUniform("time", gsio->state->time, GSIO_uniformFormat::Float);

    street = new GSIO_artist();
    GSIO_meshasset* streetMesh = gsio->assetmanager.GetMeshAsset("Street");
    street->mesh.vertices = streetMesh->vertices;
    street->mesh.indices = streetMesh->indices;
    street->mesh.GenerateMesh();
    street->transform.position = glm::vec3(0.0f,0.0f,0.0f);
    street->transform.size = glm::vec3(0.4);
    street->transform.rotation = glm::vec3(0.0f,0.0f,0.0f);
    street->materials.push_back(streetMaterial);

    streetgrassMaterial.shaderID = gsio->assetmanager.GetShaderAsset("StreetGrassShader")->ID;
    streetgrassMaterial.SetUniform("time", gsio->state->time, GSIO_uniformFormat::Float);

    streetgrass = new GSIO_artist();
    GSIO_meshasset* streetgrassMesh = gsio->assetmanager.GetMeshAsset("StreetGrass");
    streetgrass->mesh.vertices = streetgrassMesh->vertices;
    streetgrass->mesh.indices = streetgrassMesh->indices;
    streetgrass->mesh.GenerateMesh();
    streetgrass->transform.position = glm::vec3(0.0f,0.0f,0.0f);
    streetgrass->transform.size = glm::vec3(0.4);
    streetgrass->transform.rotation = glm::vec3(0.0f,0.0f,0.0f);
    streetgrass->materials.push_back(streetgrassMaterial);

    test.Load(gsio, "SoldierSh", "BarricadeSoldier", "SoldierTex");

    moneyText.Init(&gsio->engine_font, gsio);
    waveText.Init(&gsio->engine_font, gsio);
    waveText.transform.position.y = ((float)gsio->window.w_height)*0.9f;
}

void MC_Renderer::Update(GSIO *gsio)
{

    moneyText.text = "Geld: "+std::to_string(gsio->state->game.money);
    waveText.text = "Wave "+std::to_string(gsio->state->game.waveIndex)+" : "+std::to_string((int)std::floor(gsio->state->game.timeSinceWave));

    street->transform.rotation = glm::vec3(0.0f,0.0f,0.0f);
    GSIO_renderContext renderContext;
    glm::vec3 cameraPos = glm::vec3(0.0f, 5.0f, 0.0f);
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 upVector = glm::vec3(1.0f, 0.0f, 0.0f);
    renderContext.view = glm::lookAt(cameraPos, cameraTarget, upVector);
    renderContext.projection = glm::perspective(glm::radians(30.0f), (float)gsio->window.w_width/(float)gsio->window.w_height, 0.1f,100.0f);

    playerView->Clear();

    renderContext.depthTexture = &playerView->depthTexture;
    renderContext.captureTexture = &playerView->texture;
    street->materials[0].SetUniform("time", gsio->state->time, GSIO_uniformFormat::Float);
    streetgrass->materials[0].SetUniform("time", gsio->state->time, GSIO_uniformFormat::Float);

    playerView->RenderArtist(renderContext, street);
    playerView->RenderArtist(renderContext, streetgrass);

    int i = 0;  
    for (MC_Trooper* troop : gsio->state->game.troopers)
    {
        for (MC_EasyRender* render : troop->meshes)
        {
            render->model->materials[0].SetUniform("selected", gsio->state->game.selectedTroopIndex == i, GSIO_uniformFormat::Int);
            render->Render(playerView, gsio, renderContext);
        }
        i++;
    }

    for (MC_Enemy* troop : gsio->state->game.enemies)
    {
        troop->mesh->Render(playerView, gsio, renderContext);
    }

    uiView->Clear();

    GSIO_renderContext uirenderContext;
    uirenderContext.depthTexture = &uiView->depthTexture;
    uirenderContext.captureTexture = &uiView->texture;

    uirenderContext.projection = glm::ortho(0.0f, static_cast<float>(gsio->window.w_width), 0.0f, static_cast<float>(gsio->window.w_height));
    uiView->Bind();
    i = 0;
    for (MC_Trooper* troop : gsio->state->game.troopers)
    {
        troop->stats->Render(gsio, uirenderContext);
        i++;
    }
    moneyText.Render(gsio, uirenderContext);
    waveText.Render(gsio, uirenderContext);

    playerRenderer->Render(gsio, renderContext);
    uiRenderer->Render(gsio, uirenderContext);
}

void MC_RenderedText::Init(GSIO_textfont *texf, GSIO* gsio)
{
    tf = texf;
    textobj = GSIO_text(true);
    textobj.visible = true;
    mat = &gsio->engine_font_material;
    transform.size = glm::vec3(1.0f);
}

void MC_RenderedText::Render(GSIO *gsio, GSIO_renderContext &rc)
{
    textobj.text = text;
    textobj.Render(gsio, tf, mat, &transform, rc);
}

void MC_EasyRender::Load(GSIO *gsio, std::string matShader, std::string meshName, std::string texName)
{
    material.shaderID = gsio->assetmanager.GetShaderAsset(matShader)->ID;
    material.SetUniform("time", gsio->state->time, GSIO_uniformFormat::Float);

    if (texName != "")
    {
        tex.InitTexture(GSIO_textureFilter::Linear, GL_TEXTURE0, &gsio->assetmanager.GetTextureAsset(texName)->image);
        material.textures.push_back(&tex);
    }

    model = new GSIO_artist();
    GSIO_meshasset* streetgrassMesh = gsio->assetmanager.GetMeshAsset(meshName);
    model->mesh.vertices = streetgrassMesh->vertices;
    model->mesh.indices = streetgrassMesh->indices;
    model->mesh.GenerateMesh();
    model->transform.position = glm::vec3(0.0f,0.0f,0.0f);
    model->transform.size = glm::vec3(0.4);
    model->transform.rotation = glm::vec3(0.0f,0.0f,0.0f);
    model->materials.push_back(material);
}

void MC_EasyRender::Render(GSIO_capturer *view, GSIO* gsio, GSIO_renderContext &rc)
{
    model->materials[0].SetUniform("time", gsio->state->time, GSIO_uniformFormat::Float);
    view->RenderArtist(rc, model);
}

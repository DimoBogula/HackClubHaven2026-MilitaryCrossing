#include <MilitaryCrossing/Render.h>
#include <GSIO/drawing.h>
#include <GSIO/shader.h>
#include <GSIO/texture.h>
#include <GSIO/GSIO.h>
#include <GSIO/mesh.h>
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

    streetMaterial.shaderID = gsio->assetmanager.GetShaderAsset("StreetShader")->ID;
    streetMaterial.SetUniform("time", gsio->state->time, GSIO_uniformFormat::Float);

    street = new GSIO_artist();
    GSIO_meshasset* streetMesh = gsio->assetmanager.GetMeshAsset("Street");
    street->mesh.vertices = streetMesh->vertices;
    street->mesh.indices = streetMesh->indices;
    street->mesh.GenerateMesh();
    street->transform.position = glm::vec3(0.0f,0.0f,0.0f);
    street->transform.size = glm::vec3(0.5f,0.3f,0.8f);
    street->transform.rotation = glm::vec3(0.0f,0.0f,0.0f);
    street->materials.push_back(streetMaterial);
}

void MC_Renderer::Update(GSIO *gsio)
{
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

    playerView->RenderArtist(renderContext, street);

    playerRenderer->Render(gsio, renderContext);
}

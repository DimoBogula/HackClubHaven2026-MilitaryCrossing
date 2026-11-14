#include <GSIO/drawing.h>
#include <GSIO/GSIO.h>
#include <GSIO/shader.h>
#include <GL/glm/glm.hpp>
#include <GL/glm/matrix.hpp>
#include <GL/glm/gtc/type_ptr.hpp>
#include <GL/glm/ext/matrix_transform.hpp>
#include <GL/glm/ext/quaternion_float.hpp>

void GSIO_capturer::Init(GSIO* gsio)
{
    texture.width = gsio->window.w_width;
    texture.height = gsio->window.w_height;
    texture.InitTexture(GSIO_textureFilter::Pixel, GL_TEXTURE0, GL_RGBA32F, GL_RGBA, GL_FLOAT);
    texture.Bind(0);

    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.texture, 0);

    depthTexture.width = gsio->window.w_width;
    depthTexture.height = gsio->window.w_height;
    depthTexture.InitTexture(GSIO_textureFilter::Pixel, GL_TEXTURE0, GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT, GL_FLOAT);
    depthTexture.Bind(1);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture.texture, 0);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void GSIO_capturer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glViewport(0, 0, texture.width, texture.height);
}

void GSIO_capturer::Clear()
{
    Bind();
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GSIO_capturer::RenderArtist(GSIO_renderContext &rc, GSIO_artist *artist)
{
    Bind();
    for (int i = 0; i < artist->mesh.glMeshes.size(); i++)
    {
        glUseProgram(artist->materials[i].shaderID);
        artist->materials[i].SetUniforms();
        artist->materials[i].BindTextures();
        GSIO_uniform model, view, projection;
        model.format = GSIO_uniformFormat::Matrix4;
        model.name = "model";
        model.value = artist->transform.Matrix();

        view.format = GSIO_uniformFormat::Matrix4;
        view.name = "view";
        view.value = rc.view;

        projection.format = GSIO_uniformFormat::Matrix4;
        projection.name = "projection";
        projection.value = rc.projection;

        model.SetUniform(artist->materials[i].shaderID);
        view.SetUniform(artist->materials[i].shaderID);
        projection.SetUniform(artist->materials[i].shaderID);

        glBindVertexArray(artist->mesh.glMeshes[i].VAO);
        glDrawElements(GL_TRIANGLES, artist->mesh.glMeshes[i].IndexCount, GL_UNSIGNED_INT, (void*)0);
    }
    glBindVertexArray(0);
}

void GSIO_capturer::RenderQuad(GSIO_renderContext &rc, GSIO_quad *quad, GSIO_material *material, GSIO_transform *transform)
{
    Bind();
    glUseProgram(material->shaderID);
    material->SetUniforms();
    material->BindTextures();
    GSIO_uniform model, view, projection;
    model.format = GSIO_uniformFormat::Matrix4;
    model.name = "model";
    model.value = transform->Matrix();

    view.format = GSIO_uniformFormat::Matrix4;
    view.name = "view";
    view.value = rc.view;

    projection.format = GSIO_uniformFormat::Matrix4;
    projection.name = "projection";
    projection.value = rc.projection;

    model.SetUniform(material->shaderID);
    view.SetUniform(material->shaderID);
    projection.SetUniform(material->shaderID);

    quad->BuildMesh();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void GSIO_uniform::SetUniform(unsigned int shaderID)
{
    if (locationUnassigned)
    {
        location = glGetUniformLocation(shaderID, name.c_str());
        locationUnassigned = false;
    }
    switch (format)
    {
        case GSIO_uniformFormat::Matrix4:
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(std::get<glm::mat4>(value)));
        break;
        case GSIO_uniformFormat::Float:
        glUniform1f(location, std::get<float>(value));
        break;
    }
}

void GSIO_renderer::Init(GSIO *gsio)
{
}

void GSIO_renderer::Render(GSIO *gsio, GSIO_renderContext& rc)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, gsio->window.w_width, gsio->window.w_height);
    quad.size = glm::vec2(gsio->window.w_width, gsio->window.w_height);
    quad.position = glm::vec2(0.0f);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    rc.captureTexture->Bind(0);
    rc.depthTexture->Bind(1);
    shaders[0]->use();
    glUniform1i(glGetUniformLocation(shaders[0]->ID, "depth"), 1);
    glUniform1i(glGetUniformLocation(shaders[0]->ID, "image"), 0);
    glUniform1f(glGetUniformLocation(shaders[0]->ID, "time"), gsio->state->time);
    quad.Render(shaders[0], gsio);
}

glm::mat4 GSIO_transform::Matrix()
{
    glm::mat4 matrix = glm::translate(glm::mat4(1.0f), position);
    glm::quat q = glm::quat(glm::radians(rotation)); 
    matrix *= glm::mat4(q);
    matrix = glm::scale(matrix, size);
    return matrix;
}

void GSIO_material::BindTextures(uint8_t offset)
{
    for (int i = 0; i < textures.size(); i++)
        textures[i]->Bind(i+offset);
}

void GSIO_material::SetUniforms()
{
    for (int i = 0; i < uniforms.size(); i++)
        uniforms[i].SetUniform(shaderID);
}

void GSIO_material::SetUniform(std::string name, GSIO_uniformType value, GSIO_uniformFormat format)
{
    for (int i = 0; i < uniforms.size(); i++)
        if (uniforms[i].name == name)
        {
            uniforms[i].value = value;
            return;
        }

    GSIO_uniform newUniform;
    newUniform.name = name;
    newUniform.format = format;
    newUniform.value = value;
    uniforms.push_back(newUniform);
}

void GSIO_mesh::GenerateMesh()
{
    glMeshes.clear();
    GSIO_glMeshPair& mesh = glMeshes.emplace_back();

    glGenVertexArrays(1, &mesh.VAO);
    glBindVertexArray(mesh.VAO);
    glGenBuffers(1, &mesh.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
    std::vector<float> fa_vertices;
    fa_vertices.resize(vertices.size() * 8);
    for (int i = 0; i < vertices.size(); i++)
    {
        fa_vertices[i*8+0] = vertices[i].position.x;
        fa_vertices[i*8+1] = vertices[i].position.y;
        fa_vertices[i*8+2] = vertices[i].position.z;

        fa_vertices[i*8+3] = vertices[i].uv.x;
        fa_vertices[i*8+4] = vertices[i].uv.y;

        fa_vertices[i*8+5] = vertices[i].normal.x;
        fa_vertices[i*8+6] = vertices[i].normal.y;
        fa_vertices[i*8+7] = vertices[i].normal.z;
    }
    glBufferData(GL_ARRAY_BUFFER, fa_vertices.size() * sizeof(float), fa_vertices.data(), GL_STATIC_DRAW);

    GLsizei stride = 8 * sizeof(float);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3*sizeof(float)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5*sizeof(float)));
    for (int i = 0; i < 3; i++)
        glEnableVertexAttribArray(i);

    glGenBuffers(1, &mesh.EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    mesh.VertCount = vertices.size();
    mesh.IndexCount = indices.size();
}

GSIO_glMeshPair::~GSIO_glMeshPair()
{
    if (VAO != 0) glDeleteVertexArrays(1, &VAO);
    if (VBO != 0) glDeleteBuffers(1, &VBO);
    if (EBO != 0) glDeleteBuffers(1, &EBO);
}

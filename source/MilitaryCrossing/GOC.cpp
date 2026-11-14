#ifdef COMPILE_WHAT_SHOULDNT_BE
#include <GeeseOilCorp/GOC.h>
#include <GSIO/GSIO.h>
#include <GSIO/shader.h>
#include <GSIO/quad.h>
#include <GSIO/texture.h>
#include <GSIO/sound.h>
#include <algorithm>
#include <GL/glm/gtc/type_ptr.hpp>

void GeeseOilCorpStart(GSIO *gsio)
{
    gsio->gocgs = new GeeseOilCorpGlobalState();
    gsio->gocgs->Init(gsio);
}

void GeeseOilCorpUpdate(GSIO *gsio)
{
    gsio->gocgs->Update(gsio);
}

/*void GeeseOilCorpStart(GSIO *gsio)
{
    gsio->gocgs = new GeeseOilCorpGlobalState();
    gsio->gocgs->texture = new GSIO_texture();
    gsio->gocgs->quad = new GSIO_quad();
    gsio->gocgs->textureShader = gsio->assetmanager.GetShaderAsset("oscilloscopeTexture");
    gsio->gocgs->renderShader = gsio->assetmanager.GetShaderAsset("oscilloscope");
    gsio->gocgs->musicAsset = gsio->assetmanager.GetSoundAsset("osc_135.50");
    gsio->gocgs->quad->size = glm::vec2(512.0f);
    gsio->gocgs->quad->position = glm::vec2((gsio->window.w_width/2.0f)-256.0f, (gsio->window.w_height/2.0f)-256.0f);
    gsio->gocgs->texture->width = 512;
    gsio->gocgs->texture->height = 512;
    gsio->gocgs->texture->InitTexture(GSIO_textureFilter::Linear, GL_TEXTURE0, GL_R32F, GL_RED, GL_FLOAT);

    gsio->gocgs->loc_range = glGetUniformLocation(gsio->gocgs->textureShader->ID, "range");
    gsio->gocgs->loc_size = glGetUniformLocation(gsio->gocgs->textureShader->ID, "size");
    gsio->gocgs->loc_offset = glGetUniformLocation(gsio->gocgs->textureShader->ID, "offset");
    gsio->gocgs->loc_samples = glGetUniformLocation(gsio->gocgs->textureShader->ID, "samples");

    ma_engine_init(NULL, &gsio->gocgs->engine);

    ma_sound_init_from_file(&gsio->gocgs->engine, GSIO_assetmanager_Default_Path_Assets "music/osc_135.50.mp3", 0, NULL, NULL, &gsio->gocgs->sound);

    ma_sound_set_looping(&gsio->gocgs->sound, true);
    ma_sound_start(&gsio->gocgs->sound);
}

void GeeseOilCorpUpdate(GSIO *gsio)
{
    gsio->gocgs->textureShader->use();
    gsio->gocgs->texture->Bind(0);
    glUniform1f(gsio->gocgs->loc_range, 0.01f);
    glUniform2f(gsio->gocgs->loc_size, 5.0f,5.0f);
    glUniform2f(gsio->gocgs->loc_offset, 2.5f, 2.5f);
    double sampleRate = 44100.0;
    gsio->gocgs->timeSincePlayed += gsio->window.i_deltaTime;
    long unsigned long targetFrame = (long unsigned long)(gsio->gocgs->timeSincePlayed*sampleRate);
    float samples[GOC_sampleCount*2] = {};
    long long total_frames = (long long)gsio->gocgs->musicAsset->total_frames;
    for (int i = 0; i < GOC_sampleCount; i++)
    {
        long long target_frame = (long long)targetFrame - i;
        if (target_frame >= 0 && target_frame < total_frames)
        {
            samples[i * 2 + 0] = gsio->gocgs->musicAsset->pcm_data[target_frame * 2 + 0];
            samples[i * 2 + 1] = gsio->gocgs->musicAsset->pcm_data[target_frame * 2 + 1];
        }
        else
        {
            long long clamped_frame = std::clamp(target_frame, 0LL, total_frames - 1);
            samples[i * 2 + 0] = gsio->gocgs->musicAsset->pcm_data[clamped_frame * 2 + 0];
            samples[i * 2 + 1] = gsio->gocgs->musicAsset->pcm_data[clamped_frame * 2 + 1];
        }
    }
    glUniform2fv(gsio->gocgs->loc_samples, GOC_sampleCount, samples);
    glDispatchCompute((unsigned int)gsio->gocgs->texture->width/16, (unsigned int)gsio->gocgs->texture->height/16, 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    gsio->gocgs->quad->Render(gsio->gocgs->renderShader, gsio->gocgs->texture, gsio);
    gsio->gocgs->frame++;
    gsio->window.d_text.text = "frame "+std::to_string(gsio->gocgs->frame)+" at fps "+std::to_string(1.0f/gsio->window.i_deltaTime);
}*/
#endif
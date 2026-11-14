#include <GSIO/text.h>
#include <GSIO/GSIO.h>
#include <GL/glm/glm.hpp>
#include <GL/glm/gtc/matrix_transform.hpp>
#include <GL/glm/gtc/type_ptr.hpp>

GSIO_response GSIO_Load_Font(GSIO_textfont *tf, GSIO *gsio, std::string name)
{
    name += ".ttf";
    FT_Face face;
    if (FT_New_Face(gsio->ft, name.c_str(), 0, &face)) {
        return {false, GSIO_response_Error_Prefix " - Failed loading font. | "+name};
    }
    FT_Set_Pixel_Sizes(face, 0, 48);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char c = 0; c < 128; c++)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            return {false, GSIO_response_Error_Prefix " - Failed to load glyph while loading font. | "+name};
        }
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        GSIO_textfontcharacter character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<unsigned int>(face->glyph->advance.x)
        };
        tf->d_characters.insert(std::pair<char, GSIO_textfontcharacter>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    FT_Done_Face(face);
    return {true};
}

GSIO_text::GSIO_text(bool summon)
{
    if (!summon || alive) return;
    alive = summon;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

GSIO_text::~GSIO_text()
{
    return;
    if (!alive) return;
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void GSIO_text::Render(GSIO *gsio, GSIO_textfont *tf, GSIO_material *material, GSIO_transform* transform, GSIO_renderContext& rc)
{
    if (!visible) return;
    glUseProgram(material->shaderID);
    material->SetUniforms();
    material->BindTextures(1);
    glUniformMatrix4fv(glGetUniformLocation(material->shaderID, "projection"), 1, GL_FALSE, glm::value_ptr(rc.projection));
    glUniformMatrix4fv(glGetUniformLocation(material->shaderID, "view"), 1, GL_FALSE, glm::value_ptr(rc.view));
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    std::string::const_iterator c;
    glm::vec3 position = transform->position;
    for (c = text.begin(); c != text.end(); c++) 
    {
        GSIO_textfontcharacter ch = tf->d_characters[*c];

        float xpos = position.x + ch.Bearing.x * transform->size.x;
        float ypos = position.y - (ch.Size.y - ch.Bearing.y) * transform->size.y;

        float w = ch.Size.x * transform->size.x;
        float h = ch.Size.y * transform->size.y;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        position.x += (ch.Advance >> 6) * transform->size.x;
    }
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

#include <GSIO/quad.h>
#include <GSIO/GSIO.h>
#include <GSIO/shader.h>
#include <GSIO/texture.h>
#include <GL/glm/glm.hpp>
#include <GL/glm/gtc/matrix_transform.hpp>
#include <GL/glm/gtc/type_ptr.hpp>
#include <GSIO/drawing.h>

void GSIO_quad::BuildMesh()
{
    glBindVertexArray(VAO);
    float xpos = position.x;
    float ypos = position.y;

    float w = size.x;
    float h = size.y;
    if (centered)
    {
        xpos -= w/2.0f;
        ypos -= h/2.0f;
    }
    float vertices[6][4] = {
        { xpos,     ypos + h,   uv.x, uv.w },
        { xpos,     ypos,       uv.x, uv.y },
        { xpos + w, ypos,       uv.z, uv.y },

        { xpos,     ypos + h,   uv.x, uv.w },
        { xpos + w, ypos,       uv.z, uv.y },
        { xpos + w, ypos + h,   uv.z, uv.w }
    };
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GSIO_quad::Render(GSIO_shaderasset *shaderAsset, GSIO *gsio)
{
    shaderAsset->use();
    glm::mat4 projection = glm::ortho(0.0f, (float)(gsio->window.w_width), 0.0f, (float)(gsio->window.w_height));
    glUniformMatrix4fv(glGetUniformLocation(shaderAsset->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    BuildMesh();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

GSIO_quad::GSIO_quad()
{
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

GSIO_quad::~GSIO_quad()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

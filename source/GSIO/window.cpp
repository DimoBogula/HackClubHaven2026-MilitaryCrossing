#include <GSIO/GSIO.h>
#include <GSIO/window.h>
#include <GL/glm/glm.hpp>
#include <GL/glm/gtc/matrix_transform.hpp>
#include <GL/glm/gtc/type_ptr.hpp>
#include <GSIO/textureasset.h>

void GSIO_window::Render(GSIO* gsio)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glfwMakeContextCurrent(r_window);
    double currentFrame = glfwGetTime();
    i_deltaTime = static_cast<float>(currentFrame - i_pastTime);
    i_pastTime = currentFrame;
    glEnable(GL_CULL_FACE);
    GSIO_transform d_text_pos;
    d_text_pos.size = glm::vec3(1.0f);
    d_text_pos.position = glm::vec3(0.0f);
    GSIO_renderContext rc;
    rc.projection = glm::ortho(0.0f, static_cast<float>(w_width), 0.0f, static_cast<float>(w_height));
    d_text.Render(gsio, &gsio->engine_font, &gsio->engine_font_material, &d_text_pos, rc);
    glfwSwapBuffers(r_window);
}

void GSIO_window::PreRender(GSIO *gsio)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glfwMakeContextCurrent(r_window);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GSIO_window::SetIcon(GSIO_textureasset *texture, GSIO_textureasset* textureSmall)
{
    GLFWimage images[2];
    images[0].height = texture->image.height;
    images[0].width = texture->image.width;
    images[0].pixels = texture->image.image.data();

    images[1].height = textureSmall->image.height;
    images[1].width = textureSmall->image.width;
    images[1].pixels = textureSmall->image.image.data();

    glfwSetWindowIcon(r_window, 2, images);
}

GSIO_window::GSIO_window(uint32_t w, uint32_t h, std::string n)
{
    w_name = n;
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    w_width = mode->width;
    w_height = mode->height;

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    glfwWindowHint(GLFW_ALPHA_BITS, 8);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);
	glfwWindowHint(GLFW_STENCIL_BITS, 8);
 
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
    
    r_window = glfwCreateWindow(mode->width, mode->height, n.c_str(), monitor, NULL);
    glfwMakeContextCurrent(r_window);
}

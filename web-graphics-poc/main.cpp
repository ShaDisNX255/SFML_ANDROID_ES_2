#include <SFML/Graphics.hpp>

#include <emscripten/emscripten.h>

#include <cstdio>

namespace
{
    sf::RenderWindow* g_window = NULL;
    sf::Shader* g_defaultShader = NULL;
    sf::Texture* g_texture = NULL;
    sf::Sprite* g_sprite = NULL;
    bool g_firstFrameLogged = false;

    const char* VertexShader =
        "attribute vec2 position;\n"
        "attribute vec4 color;\n"
        "attribute vec2 texCoord;\n"
        "\n"
        "uniform mat4 projMatrix;\n"
        "uniform mat4 viewMatrix;\n"
        "uniform mat4 textMatrix;\n"
        "\n"
        "varying vec4 fragColor;\n"
        "varying vec2 fragTexCoord;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    gl_Position = projMatrix * viewMatrix * vec4(position, 0.0, 1.0);\n"
        "    fragColor = color;\n"
        "    fragTexCoord = (textMatrix * vec4(texCoord, 0.0, 1.0)).xy;\n"
        "}\n";

    const char* FragmentShader =
        "precision mediump float;\n"
        "\n"
        "uniform sampler2D texture0;\n"
        "\n"
        "varying vec4 fragColor;\n"
        "varying vec2 fragTexCoord;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    gl_FragColor = texture2D(texture0, fragTexCoord) * fragColor;\n"
        "}\n";

    void frame()
    {
        if (!g_window || !g_window->isOpen())
        {
            std::fprintf(stderr, "[ONB_WEB_BOOT] RenderWindow closed; stopping main loop\n");
            emscripten_cancel_main_loop();
            return;
        }

        g_window->clear(sf::Color(18, 28, 46));

        if (g_sprite)
            g_window->draw(*g_sprite);

        g_window->display();

        if (!g_firstFrameLogged)
        {
            std::fprintf(stderr, "[ONB_WEB_GL] SFML sprite frame rendered successfully\n");
            g_firstFrameLogged = true;

            // This is a static rendering test. One frame is enough.
            // Stop here so a GL failure cannot flood the browser console.
            emscripten_cancel_main_loop();
        }
    }
}

int main()
{
    std::fprintf(stderr, "[ONB_WEB_BOOT] SFML graphics browser test started\n");
    std::fprintf(stderr, "[ONB_WEB_GL] Creating sf::RenderWindow\n");

    const sf::ContextSettings settings(
        0,
        0,
        0,
        2,
        0
    );

    g_window = new sf::RenderWindow(
        sf::VideoMode(640, 480, 32),
        "OpenNetBattle SFML Graphics Web PoC",
        sf::Style::Default,
        settings
    );

    if (!g_window->isOpen())
    {
        std::fprintf(stderr, "[ONB_WEB_GL] ERROR: sf::RenderWindow creation failed\n");
        return 1;
    }

    std::fprintf(
        stderr,
        "[ONB_WEB_GL] sf::RenderWindow created: %ux%u\n",
        g_window->getSize().x,
        g_window->getSize().y
    );

    std::fprintf(stderr, "[ONB_WEB_GL] Creating GLES2 default SFML shader\n");

    g_defaultShader = new sf::Shader();

    if (!g_defaultShader->loadFromMemory(VertexShader, FragmentShader))
    {
        std::fprintf(stderr, "[ONB_WEB_GL] ERROR: default GLES2 shader failed to compile/link\n");
        return 2;
    }

    g_defaultShader->setUniform("texture0", sf::Shader::CurrentTexture);
    g_window->setDefaultShader(g_defaultShader);

    std::fprintf(
        stderr,
        "[ONB_WEB_GL] Default shader installed, program ID: %u\n",
        g_defaultShader->getNativeHandle()
    );

    std::fprintf(stderr, "[ONB_WEB_FS] Loading /assets/test.png through sf::Texture\n");

    g_texture = new sf::Texture();

    if (!g_texture->loadFromFile("/assets/test.png"))
    {
        std::fprintf(stderr, "[ONB_WEB_FS] ERROR: sf::Texture::loadFromFile failed\n");
        return 3;
    }

    std::fprintf(
        stderr,
        "[ONB_WEB_FS] Texture loaded successfully: %ux%u\n",
        g_texture->getSize().x,
        g_texture->getSize().y
    );

    g_sprite = new sf::Sprite(*g_texture);
    g_sprite->setScale(3.0f, 3.0f);

    const float spriteWidth = static_cast<float>(g_texture->getSize().x) * 3.0f;
    const float spriteHeight = static_cast<float>(g_texture->getSize().y) * 3.0f;

    g_sprite->setPosition(
        (640.0f - spriteWidth) * 0.5f,
        (480.0f - spriteHeight) * 0.5f
    );

    std::fprintf(stderr, "[ONB_WEB_GL] Sprite prepared and centered\n");
    std::fprintf(stderr, "[ONB_WEB_BOOT] Entering browser render loop\n");

    emscripten_set_main_loop(frame, 0, 1);

    return 0;
}

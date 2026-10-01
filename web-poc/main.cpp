#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

#include <emscripten/emscripten.h>

#include <cstdio>

namespace
{
    sf::Window* g_window = NULL;
    bool g_renderLogged = false;

    void frame()
    {
        if (!g_window || !g_window->isOpen())
        {
            std::fprintf(stderr, "[ONB_WEB_BOOT] Window is no longer open; stopping main loop\n");
            emscripten_cancel_main_loop();
            return;
        }

        if (!g_window->setActive(true))
        {
            std::fprintf(stderr, "[ONB_WEB_GL] ERROR: Could not activate SFML WebGL context\n");
            emscripten_cancel_main_loop();
            return;
        }

        const sf::Vector2u size = g_window->getSize();

        glViewport(0, 0, static_cast<GLsizei>(size.x), static_cast<GLsizei>(size.y));
        glClearColor(0.08f, 0.18f, 0.36f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        g_window->display();

        if (!g_renderLogged)
        {
            std::fprintf(
                stderr,
                "[ONB_WEB_GL] Frame rendered successfully at %ux%u\n",
                size.x,
                size.y
            );
            g_renderLogged = true;
        }
    }
}

int main()
{
    std::fprintf(stderr, "[ONB_WEB_BOOT] Browser canvas test started\n");
    std::fprintf(stderr, "[ONB_WEB_GL] Creating SFML window\n");

    const sf::ContextSettings settings(
        0,  // depth
        0,  // stencil
        0,  // antialiasing
        2,  // OpenGL ES major
        0   // OpenGL ES minor
    );

    g_window = new sf::Window(
        sf::VideoMode(640, 480, 32),
        "OpenNetBattle Web PoC",
        sf::Style::Default,
        settings
    );

    if (!g_window->isOpen())
    {
        std::fprintf(stderr, "[ONB_WEB_GL] ERROR: SFML window creation failed\n");
        return 1;
    }

    std::fprintf(
        stderr,
        "[ONB_WEB_GL] SFML window created: %ux%u\n",
        g_window->getSize().x,
        g_window->getSize().y
    );

    if (!g_window->setActive(true))
    {
        std::fprintf(stderr, "[ONB_WEB_GL] ERROR: Initial context activation failed\n");
        return 2;
    }

    std::fprintf(stderr, "[ONB_WEB_GL] WebGL context activated\n");

    // Browser applications must yield control back to the browser each frame.
    emscripten_set_main_loop(frame, 0, 1);

    return 0;
}

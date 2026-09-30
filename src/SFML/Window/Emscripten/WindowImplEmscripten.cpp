#include <SFML/Window/Emscripten/WindowImplEmscripten.hpp>

#include <emscripten/html5.h>
#include <cstdio>

namespace sf
{
namespace priv
{

WindowImplEmscripten::WindowImplEmscripten(WindowHandle)
: m_size(0, 0)
{
    int width = 0;
    int height = 0;

    emscripten_get_canvas_element_size("#canvas", &width, &height);

    if (width > 0 && height > 0)
        m_size = Vector2u(static_cast<unsigned int>(width), static_cast<unsigned int>(height));

    std::fprintf(stderr,
        "[ONB_WEB_BOOT] SFML Emscripten window attached: %ux%u\n",
        m_size.x,
        m_size.y);
}

WindowImplEmscripten::WindowImplEmscripten(
    VideoMode mode,
    const String&,
    Uint32,
    const ContextSettings&)
: m_size(mode.width, mode.height)
{
    emscripten_set_canvas_element_size(
        "#canvas",
        static_cast<int>(m_size.x),
        static_cast<int>(m_size.y)
    );

    std::fprintf(stderr,
        "[ONB_WEB_BOOT] SFML Emscripten canvas created: %ux%u\n",
        m_size.x,
        m_size.y);
}

WindowImplEmscripten::~WindowImplEmscripten()
{
}

WindowHandle WindowImplEmscripten::getSystemHandle() const
{
    // Emscripten EGL does not require a native window object.
    return 0;
}

Vector2i WindowImplEmscripten::getPosition() const
{
    return Vector2i(0, 0);
}

void WindowImplEmscripten::setPosition(const Vector2i&)
{
}

Vector2u WindowImplEmscripten::getSize() const
{
    return m_size;
}

void WindowImplEmscripten::setSize(const Vector2u& size)
{
    m_size = size;

    emscripten_set_canvas_element_size(
        "#canvas",
        static_cast<int>(size.x),
        static_cast<int>(size.y)
    );
}

void WindowImplEmscripten::setTitle(const String&)
{
}

void WindowImplEmscripten::setIcon(unsigned int, unsigned int, const Uint8*)
{
}

void WindowImplEmscripten::setVisible(bool)
{
}

void WindowImplEmscripten::setMouseCursorVisible(bool)
{
}

void WindowImplEmscripten::setMouseCursorGrabbed(bool)
{
}

void WindowImplEmscripten::setMouseCursor(const CursorImpl&)
{
}

void WindowImplEmscripten::setKeyRepeatEnabled(bool)
{
}

void WindowImplEmscripten::requestFocus()
{
}

bool WindowImplEmscripten::hasFocus() const
{
    return true;
}

void WindowImplEmscripten::processEvents()
{
    // Browser events will be registered with Emscripten callbacks later.
}

} // namespace priv
} // namespace sf

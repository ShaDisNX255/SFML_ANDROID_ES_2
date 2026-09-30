#include <SFML/Window/Emscripten/InputImpl.hpp>

namespace sf
{
namespace priv
{

bool InputImpl::isKeyPressed(Keyboard::Key)
{
    return false;
}

void InputImpl::setVirtualKeyboardVisible(bool)
{
}

bool InputImpl::isMouseButtonPressed(Mouse::Button)
{
    return false;
}

Vector2i InputImpl::getMousePosition()
{
    return Vector2i(0, 0);
}

Vector2i InputImpl::getMousePosition(const Window&)
{
    return getMousePosition();
}

void InputImpl::setMousePosition(const Vector2i&)
{
}

void InputImpl::setMousePosition(const Vector2i&, const Window&)
{
}

bool InputImpl::isTouchDown(unsigned int)
{
    return false;
}

Vector2i InputImpl::getTouchPosition(unsigned int)
{
    return Vector2i(0, 0);
}

Vector2i InputImpl::getTouchPosition(unsigned int, const Window&)
{
    return Vector2i(0, 0);
}

} // namespace priv
} // namespace sf

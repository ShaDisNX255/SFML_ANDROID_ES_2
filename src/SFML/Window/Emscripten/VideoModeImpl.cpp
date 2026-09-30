#include <SFML/Window/VideoModeImpl.hpp>
#include <emscripten/html5.h>

namespace sf
{
namespace priv
{

std::vector<VideoMode> VideoModeImpl::getFullscreenModes()
{
    std::vector<VideoMode> modes;
    modes.push_back(getDesktopMode());
    return modes;
}

VideoMode VideoModeImpl::getDesktopMode()
{
    int width = 0;
    int height = 0;

    emscripten_get_canvas_element_size("#canvas", &width, &height);

    if (width <= 0 || height <= 0)
    {
        width = 640;
        height = 480;
    }

    return VideoMode(
        static_cast<unsigned int>(width),
        static_cast<unsigned int>(height),
        32
    );
}

} // namespace priv
} // namespace sf

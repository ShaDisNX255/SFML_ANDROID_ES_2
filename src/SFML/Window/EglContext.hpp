////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2013 Jonathan De Wachter (dewachter.jonathan@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

#ifndef SFML_EGLCONTEXT_HPP
#define SFML_EGLCONTEXT_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/ContextSettings.hpp>
#include <SFML/Window/EGLCheck.hpp>
#include <SFML/Window/GlContext.hpp>
#include <SFML/OpenGL.hpp>


namespace sf
{
namespace priv
{
class EglContext : public GlContext
{
public:

    EglContext(EglContext* shared);
    EglContext(EglContext* shared, const ContextSettings& settings, const WindowImpl* owner, unsigned int bitsPerPixel);
    EglContext(EglContext* shared, const ContextSettings& settings, unsigned int width, unsigned int height);
    ~EglContext();

    virtual bool makeCurrent(bool current);
    virtual void display();
    virtual void setVerticalSyncEnabled(bool enabled);

    void createContext(EglContext* shared);
    void createSurface(EGLNativeWindowType window);
    void destroySurface();

    static EGLConfig getBestConfig(EGLDisplay display, unsigned int bitsPerPixel, const ContextSettings& settings);

#ifdef SFML_SYSTEM_LINUX
    static XVisualInfo selectBestVisual(::Display* display, unsigned int bitsPerPixel, const ContextSettings& settings);
#endif

private:

    void updateSettings();

    EGLDisplay  m_display;
    EGLContext  m_context;
    EGLSurface  m_surface;
    EGLConfig   m_config;

    // Emscripten exposes one browser WebGL/EGL context. Wrapper contexts
    // therefore alias the shared browser context and must not destroy it.
    bool m_ownsContext;
};

} // namespace priv
} // namespace sf

#endif // SFML_EGLCONTEXT_HPP

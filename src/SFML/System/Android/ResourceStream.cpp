////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2013 Jonathan De Wachter (dewachter.jonathan@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from
// the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/System/Android/ResourceStream.hpp>
#include <SFML/System/Android/Activity.hpp>
#include <SFML/System/Lock.hpp>
#include <cstdio>


namespace sf
{
namespace priv
{

////////////////////////////////////////////////////////////
ResourceStream::ResourceStream(const std::string& filename) :
m_file       (NULL),
m_regularFile(NULL)
{
    // ONB_ANDROID_RESOURCESTREAM:
    // First try normal filesystem paths. This is needed for Android external
    // app storage, for example extracted mods under:
    // /storage/emulated/0/Android/data/.../OpenNetBattle/resources/mods/...
    m_regularFile = std::fopen(filename.c_str(), "rb");

    if (m_regularFile)
    {
        return;
    }

    // Fall back to APK assets for bundled resources.
    ActivityStates* states = getActivity(NULL);

    if (!states || !states->activity || !states->activity->assetManager)
    {
        return;
    }

    Lock lock(states->mutex);
    m_file = AAssetManager_open(states->activity->assetManager, filename.c_str(), AASSET_MODE_UNKNOWN);
}


////////////////////////////////////////////////////////////
ResourceStream::~ResourceStream()
{
    if (m_regularFile)
    {
        std::fclose(m_regularFile);
        m_regularFile = NULL;
    }

    if (m_file)
    {
        AAsset_close(m_file);
        m_file = NULL;
    }
}


////////////////////////////////////////////////////////////
Int64 ResourceStream::read(void *data, Int64 size)
{
    if (m_regularFile)
    {
        if (size <= 0)
        {
            return 0;
        }

        return static_cast<Int64>(
            std::fread(data, 1, static_cast<std::size_t>(size), m_regularFile)
        );
    }

    if (m_file)
    {
        return AAsset_read(m_file, data, size);
    }

    return -1;
}


////////////////////////////////////////////////////////////
Int64 ResourceStream::seek(Int64 position)
{
    if (m_regularFile)
    {
        if (std::fseek(m_regularFile, static_cast<long>(position), SEEK_SET) != 0)
        {
            return -1;
        }

        return tell();
    }

    if (m_file)
    {
        return AAsset_seek(m_file, position, SEEK_SET);
    }

    return -1;
}


////////////////////////////////////////////////////////////
Int64 ResourceStream::tell()
{
    if (m_regularFile)
    {
        long position = std::ftell(m_regularFile);

        if (position < 0)
        {
            return -1;
        }

        return static_cast<Int64>(position);
    }

    if (m_file)
    {
        return getSize() - AAsset_getRemainingLength(m_file);
    }

    return -1;
}


////////////////////////////////////////////////////////////
Int64 ResourceStream::getSize()
{
    if (m_regularFile)
    {
        long current = std::ftell(m_regularFile);

        if (current < 0)
        {
            return -1;
        }

        if (std::fseek(m_regularFile, 0, SEEK_END) != 0)
        {
            return -1;
        }

        long size = std::ftell(m_regularFile);

        if (std::fseek(m_regularFile, current, SEEK_SET) != 0)
        {
            return -1;
        }

        if (size < 0)
        {
            return -1;
        }

        return static_cast<Int64>(size);
    }

    if (m_file)
    {
        return AAsset_getLength(m_file);
    }

    return -1;
}


} // namespace priv
} // namespace sf

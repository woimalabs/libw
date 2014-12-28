/**
 * libw
 *
 * Copyright (C) 2013-2014 Woima Solutions Oy
 *
 * This software is provided 'as-is', without any express or implied warranty. In
 * no event will the authors be held liable for any damages arising from the use
 * of this software.
 *
 * Permission is granted to anyone to use this software for any purpose, including
 * commercial applications, and to alter it and redistribute it freely, subject to
 * the following restrictions:
 *
 * 1) The origin of this software must not be misrepresented; you must not claim
 *    that you wrote the original software. If you use this software in a product,
 *    an acknowledgment in the product documentation is appreciated.
 *
 * 2) Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 *
 * 3) This notice may not be removed or altered from any source distribution.
 *
 * @author antti.peuhkurinen@woimasolutions.com
 */

#ifndef LIBW_AUDIO_AUDIOENGINE
#define LIBW_AUDIO_AUDIOENGINE

#include <w/base/ResourceManager.hpp>
#include <w/base/Class.hpp>
#include <string>

/**
 * AudioEngine is a class that manages audio.
 */
namespace w
{
    namespace audio
    {
        class AudioEngine
        {
        public:
            UNCOPYABLE(AudioEngine)

            /**
             * Creates AudioEngine instance which makes possible to play AudioAssets.
             *
             * @param [in]  mute            Is mute on or off at start.
             * @param [in]  resourceManager Manages loading of files
             */
            AudioEngine(bool mute, ResourceManager& resourceManager);
            ~AudioEngine();
            static void setMute(bool);
            static bool mute();

        private:
            static class AudioEnginePrivate* private_;
        };
    }
}

#endif

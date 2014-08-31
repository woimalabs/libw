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

#ifndef LIBW_AUDIO_AUDIOASSET
#define LIBW_AUDIO_AUDIOASSET

#include <w/base/Class.hpp>
#include <string>

/**
 * AudioAsset is a class for sound samples.
 *
 * Supported format(s):
 *  - wav, PCM mono 44100Hz 16bit, signed integer, linear, little endian
 */
namespace w
{
    namespace audio
    {
        class AudioAsset
        {
        public:
            COPYABLE(AudioAsset)

            /**
             * Creates audio asset
             *
             * @note Resources are automatically shared meaning that same audio resource
             * file is loaded only once.
             *
             * @param [in]  filename    filename asset
             */
            AudioAsset(const std::string& filename, bool parallelPlay = false, bool looping = false);
            ~AudioAsset();

            /**
             * Plays the sample.
             *
             * @param [in]  volume      Volume level [0.0 - 1.0], value is checked & limited.
             * @return                  TRUE if sample started to play.
             *                          FALSE if tracks were full and sample could not be played.
             *
             * @note AudioEngine must exist to succesfully make a play() call.
             */
            bool play(float volume = 1.0f);

            /**
             * Set volume level.
             *
             * @param [in]  volume      Volume level [0.0 - 1.0], value is checked & limited.
             */
            void setVolume(float volume);

            /**
             * Fades the sample away with linear fade.
             *
             * @param [in]  fadeOutTime     Millisecond time to fadeout the sound.
             */
            void fadeOut(unsigned int fadeOutTimeMilliseconds = 0);

        private:
            class AudioAssetPrivate* private_;
        };
    }
}

#endif

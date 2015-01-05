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

#ifndef LIBW_AUDIO_TRACKERSAMPLE
#define LIBW_AUDIO_TRACKERSAMPLE

#include "AudioResource.hpp"
#include <w/base/ReferencedPointer.hpp>
#include <w/base/Mutex.hpp>
#include <sigc++/connection.h>
#include <stdint.h>

namespace w
{
    namespace audio
    {
        class TrackerSample: public Referenced
        {
        public:
            static unsigned int const BytesPerSample = 2;

            sigc::signal<void, unsigned int /* Referenced::id */> ended;
            sigc::connection audioAssetInterestedFromEnd;

            TrackerSample(const ReferencedPointer<AudioResource>& resource, float volume, bool looping);
            ~TrackerSample();
            float volume();
            void setVolume(float volume);
            int16_t sampleForTracker(bool& end);
            void audioAssetSetNotInterested();
            void fadeOut(unsigned int fadeTimeMilliseconds);

        private:
            // Mutex is to used to control the "ended" signal connection to parent AudioAssetPrivate
            Mutex mutex_;
            float volume_;
            ReferencedPointer<AudioResource> resource_;
            unsigned int byteSize_;
            unsigned int byteLocation_;
            bool looping_;

            struct FadeOut
            {
                bool on_;
                unsigned int start_;
                float ramp_; // applied every sample() call to volume_ during fadeout
            } fadeOut_;
        };
    }
}

#endif

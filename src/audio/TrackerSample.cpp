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

#include "TrackerSample.hpp"
#include "AudioResource.hpp"
#include "w/base/Timer.hpp"
#include "w/base/Log.hpp"

namespace w
{
    namespace audio
    {
        TrackerSample::TrackerSample(const ReferencedPointer<AudioResource>& resource, float volume, bool looping):
            Referenced(),
            volume_(volume),
            resource_(resource),
            byteSize_(resource.pointer()->sizeInBytes()),
            byteLocation_(0),
            looping_(looping)
        {
            fadeOut_.on_ = false;
            fadeOut_.start_ = 0;
        }

        TrackerSample::~TrackerSample()
        {
        }

        float TrackerSample::volume()
        {
            return volume_;
        }

        void TrackerSample::setVolume(float volume)
        {
            volume_ = volume;
        }

        int16_t TrackerSample::sampleForTracker(bool& end)
        {
            int16_t r = resource_.pointer()->sample(byteLocation_, end);
            byteLocation_ += BytesPerSample;

            if (looping_ == true && byteLocation_ >= byteSize_)
            {
                byteLocation_ = 0;
                end = false;
            }

            if (fadeOut_.on_ == true)
            {
                volume_ -= fadeOut_.ramp_;
                if (volume_ <= 0.0f)
                {
                    end = true;
                    // we are not interested from this end at parent side, because
                    // it is a fadeout end and we have disconnected already from "ended"
                    volume_ = 0.0f;
                }
                r = int16_t(volume_ * (float)r);
            }
            else
            {
                r = int16_t(volume_ * (float)r);
                if(end == true)
                {
                    LOCK
                    ended.emit(id());
                }
            }

            return r;
        }

        void TrackerSample::fadeOut(unsigned int fadeTimeMilliseconds)
        {
            LOCK

            if (fadeOut_.on_ == false)
            {
                audioAssetInterestedFromEnd.disconnect();
                if (fadeTimeMilliseconds == 0)
                {
                    fadeOut_.start_ = Timer::milliseconds();
                    fadeOut_.ramp_ = 1.0f;
                    fadeOut_.on_ = true;
                }
                else
                {
                    fadeOut_.start_ = Timer::milliseconds();
                    fadeOut_.ramp_ = 1.0f / ((float)resource_.pointer()->playBackRate() / 1000.0f * (float)fadeTimeMilliseconds);
                    fadeOut_.on_ = true;
                }
            }
        }
    }
}

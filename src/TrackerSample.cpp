/**
 * libw
 *
 * Copyright (C) 2013 Woima Solutions
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
#include "Timer.hpp"
#include "w/Log.hpp"

namespace w
{
    TrackerSample::TrackerSample(AudioResource* resource, float volume, bool looping):
        Referenced(),
        volume_(volume),
        resource_(resource),
        byteSize_(resource->sizeInBytes()),
        byteLocation_(0),
        looping_(looping)
    {
        fadeOut_.on_ = false;
        fadeOut_.start_ = 0;
        resource_->increment();
    }

    TrackerSample::~TrackerSample()
    {
        LOG
        if (resource_ != NULL)
        {
            resource_->decrement();
        }
    }

    float TrackerSample::volume()
    {
        return volume_;
    }

    void TrackerSample::setVolume(float volume)
    {
        volume_ = volume;
    }

    int16_t TrackerSample::sample(bool& end)
    {
        int16_t r = resource_->sample(byteLocation_, end);
        byteLocation_ += BytesPerSample;

        if (looping_ == true && byteLocation_ >= byteSize_)
        {
            byteLocation_ = 0;
            end = false;
        }

        if (fadeOut_.on_ == true)
        {
            volume_ -= fadeOut_.ramp_;
            if (volume_ < 0.0f)
            {
                end = true;
                volume_ = 0.0f;
            }
            r = int16_t(volume_ * (float)r);
        }
        else
        {
            r = int16_t(volume_ * (float)r);
        }

        return r;
    }

    void TrackerSample::fadeOut(unsigned int fadeTimeMilliseconds)
    {
        if (fadeOut_.on_ == false)
        {
            if (fadeTimeMilliseconds == 0)
            {
                volume_ = 0.0f;
            }
            else
            {
                fadeOut_.start_ = Timer::milliseconds();
                fadeOut_.ramp_ = 1.0f / ((float)resource_->playBackRate() / 1000.0f * (float)fadeTimeMilliseconds);
                fadeOut_.on_ = true;
            }
        }
    }
}

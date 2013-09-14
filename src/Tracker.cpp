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

#include "Tracker.hpp"
#include "Timer.hpp"
#include "Lock.hpp"
#include "w/Exception.hpp"
#include "w/Log.hpp"

namespace w
{
    Tracker::Tracker(float volume):
        shutdownStarted_(false),
        shutdownDone_(false)
    {
    }

    Tracker::~Tracker()
    {
    }

    void Tracker::shutdown()
    {
        LOCK

        if (shutdownStarted_ == false)
        {
            for (unsigned int i = 0; i < Tracker::TrackAmount; i++)
            {
                if (tracks_[i] != NULL)
                    tracks_[i]->fadeOut(100);
            }

            shutdownStarted_ = true;
        }
    }

    bool Tracker::shutdownIsDone()
    {
        return shutdownDone_;
    }

    bool Tracker::place(TrackerSample* trackerSample)
    {
        bool r = false;

        LOCK
        if (shutdownStarted_ == false)
        {
            for (unsigned int i = 0; i < Tracker::TrackAmount; i++)
            {
                if (tracks_[i] == NULL)
                {
                    tracks_[i] = trackerSample;
                    r = true;
                    break;
                }
            }
        }

        return r;
    }

    unsigned int Tracker::data(unsigned int size, unsigned char* data)
    {
        LOCK

        bool tracksWithAudio = false;

        for (unsigned int i = 0; i < size; i += 2)
        {
            int a = 0;
            for (unsigned int t = 0; t < Tracker::TrackAmount; t++)
            {
                if (tracks_[t] != NULL)
                {
                    tracksWithAudio = true;
                    bool end = false;
                    a += tracks_[t]->sample(end);
                    if (end == true)
                    {
                        tracks_[t]->decrement();
                        tracks_[t] = NULL;
                    }
                }
            }
            a = (float)a / (float)Tracker::TrackAmount;

            // Put to data
            data[i] = ((char*)&a)[0];
            data[i+1] = ((char*)&a)[1];
        }

        if (shutdownStarted_ && tracksWithAudio == false)
        {
            shutdownDone_ = true;
        }

        return size;
    }

    void Tracker::setVolume(float volume)
    {
        // TODO
    }

    float Tracker::volume()
    {
        return 1.0f; // TODO
    }
}

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

#include "Tracker.hpp"
#include "w/base/Timer.hpp"
#include "w/base/Lock.hpp"
#include "w/base/Exception.hpp"
#include "w/base/Log.hpp"
#include "w/base/Helpers.hpp"
#include <stdint.h>

namespace w
{
    namespace audio
    {
        Tracker::Tracker(bool muteAtStart):
            mute_(muteAtStart),
            shutdownStarted_(false),
            shutdownDone_(false),
            ringBuffer_(44100 * 2 / 10), /* one second divided by 10 to have max 100ms lag here */
            producerThread_(this)
        {
            /*for(unsigned int i = 0; i < Tracker::TrackAmount; i++)
            {
                tracks_[i] = NULL;
            }*/
            producerThread_.start();
        }

        Tracker::~Tracker()
        {
        }

        void Tracker::shutdown()
        {
            LOCK

            if(shutdownStarted_ == false)
            {
                for (unsigned int i = 0; i < Tracker::TrackAmount; i++)
                {
                    if(tracks_[i].pointer() != NULL)
                    {
                        tracks_[i].pointer()->fadeOut(100);
                    }
                }

                shutdownStarted_ = true;
            }
        }

        bool Tracker::shutdownIsDone()
        {
            return shutdownDone_;
        }

        bool Tracker::place(ReferencedPointer<TrackerSample> const& trackerSample)
        {
            bool r = false;

            LOCK

            if(shutdownStarted_ == false)
            {
                for (unsigned int i = 0; i < Tracker::TrackAmount; i++)
                {
                    if(tracks_[i].pointer() == NULL)
                    {
                        tracks_[i] = trackerSample;
                        //tracks_[i]->increment();
                        r = true;
                        break;
                    }
                }
            }

            return r;
        }

        unsigned int Tracker::getData(unsigned int size, unsigned char* data)
        {
            unsigned int r = 0;

            for (unsigned int i = 0; i < size; i += 2)
            {
                int16_t a = 0;
                while (ringBuffer_.get(a) == false)
                {
                    if(shutdownStarted_ == true)
                    {
                        //LOG
                        break;
                    }
                }
                data[i] = ((char*)&a)[0];
                data[i+1] = ((char*)&a)[1];
                r = i;
            }

            return r;
        }

        bool Tracker::produceData()
        {
            LOCK

            bool r = true;
            unsigned int bytesProduced = 0;
            bool mute = mute_;

            while (true)
            {
                if(ringBuffer_.full() == true)
                {
                    //LOGD("Tracker::produceData(), ring buffer is full.");
                    break;
                }

                int a = 0;
                bool tracksWithAudio = false;
                for(unsigned int t = 0; t < Tracker::TrackAmount; t++)
                {
                    if(tracks_[t].pointer() != NULL)
                    {
                        tracksWithAudio = true;
                        bool end = false;
                        a += tracks_[t].pointer()->sampleForTracker(end);
                        if(end == true)
                        {
                            tracks_[t] = ReferencedPointer<TrackerSample>(NULL);
                        }
                    }
                }
                if(mute)
                {
                    a = 0;
                }
                else
                {
                    a = (float)a / (float)Tracker::TrackAmount;
                }

                // Put to data
                int16_t data = a;
                bytesProduced += 2;

                if(shutdownStarted_ && tracksWithAudio == false)
                {
                    r = false;
                    shutdownDone_ = true;
                    break; // buffer is full OR we are shutting down
                }

                if(ringBuffer_.put(data) == false)
                {
                    throw Exception("Tracker::produceData(), ring buffer was full even if we checked that it is not!");
                    break;
                }
            }

            //LOGD("bytes produced: %d", bytesProduced);
            return r;
        }

        void volumeZeroProcess()
        {
            // TODO
        }

        void Tracker::setMute(bool value)
        {
            mute_ = value;
        }

        bool Tracker::mute()
        {
            return mute_;
        }
    }
}

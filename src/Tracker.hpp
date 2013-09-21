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

#ifndef LIBW_TRACKER
#define LIBW_TRACKER

#include "TrackerSample.hpp"
#include "Mutex.hpp"
#include "TrackerProducerThread.hpp"
#include "w/Class.hpp"
#include "RingBuffer.hpp"
#include <stdint.h>

namespace w
{
    class Tracker
    {
    public:
        UNCOPYABLE(Tracker);

        static unsigned int const TrackAmount = 4;
        static unsigned int const VolumeTransitionMilliSeconds = 450;
        static float const VolumeOffThreshold = 0.001f;

        Tracker(float volumeAtStart);
        ~Tracker();

        /**
         * Ends playing of TrackerSamples on Tracks so that eventually
         * no TrackerSamples exist and AudioSystemPrivate destructor
         * can proceed.
         */
        void shutdown();
        bool shutdownIsDone();
        bool place(TrackerSample* trackerSample);
        unsigned int getData(unsigned int size, unsigned char* data);
        bool produceData();
        void setVolume(float volume);
        float volume();

    private:
        Mutex mutex_;
        TrackerSample* tracks_[TrackAmount];
        bool shutdownStarted_;
        bool shutdownDone_;
        RingBuffer<int16_t> ringBuffer_;
        TrackerProducerThread producerThread_;
    };
}

#endif

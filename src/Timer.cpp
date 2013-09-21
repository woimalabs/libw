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

#include "Timer.hpp"
#include <time.h>
#include <unistd.h>
#include <ctime>
#ifdef __APPLE__
#include <mach/mach_time.h>
#endif
#include "w/Exception.hpp"
#include "w/Log.hpp"

namespace w
{
    Timer::Timer()
    {
    }

    Timer::~Timer()
    {
    }

    unsigned int Timer::milliseconds()
    {
    #ifdef __APPLE__
        return nanoseconds() / 1000000LL;
    #else // Android and Linux
        struct timespec now;
        clock_gettime(CLOCK_MONOTONIC, &now);
        return now.tv_sec * 1000LL + now.tv_nsec / 1000000LL;
    #endif
    }

    uint64_t Timer::nanoseconds()
    {
    #ifdef __APPLE__
        mach_timebase_info_data_t info;
        mach_timebase_info(&info);

        uint64_t tmp = mach_absolute_time();
        tmp *= info.numer;
        tmp /= info.denom;

        return tmp;
    #else // Android and Linux
        struct timespec now;
        clock_gettime(CLOCK_MONOTONIC, &now);
        return now.tv_sec * 1000000000LL + now.tv_nsec;
    #endif
    }

    void Timer::sleepMilliseconds(unsigned int ms)
    {
        nanoSleepFromMilliseconds(ms);
    }

    void Timer::nanoSleepFromMilliseconds(unsigned int milliseconds)
    {
        struct timespec tim, tim2;
        tim.tv_sec = milliseconds / 1000;
        tim.tv_nsec = (milliseconds % 1000) * 1000000LL;

        if (nanosleep(&tim , &tim2) < 0)
        {
            throw Exception("Timer::nanoSleep call to nanosleep() failed.");
        }
    }
}

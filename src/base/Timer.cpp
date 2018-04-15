
/**
 * libw
 *
 * Copyright (C) 2012-2018 Woima Solutions
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

#include "w/base/Timer.hpp"
#include <chrono>
#include "w/base/Exception.hpp"
#include "w/base/Log.hpp"

namespace w
{
    static std::chrono::milliseconds millisecondsSinceEpoch()
    {
        std::chrono::system_clock::time_point t = std::chrono::system_clock::now();
        const std::chrono::duration<double> timeSinceEpoch = t.time_since_epoch();
        return std::chrono::duration_cast<std::chrono::milliseconds>(timeSinceEpoch);
    }
    static std::chrono::nanoseconds nanosecondsSinceEpoch()
    {
        std::chrono::system_clock::time_point t = std::chrono::system_clock::now();
        const std::chrono::duration<double> timeSinceEpoch = t.time_since_epoch();
        return std::chrono::duration_cast<std::chrono::nanoseconds>(timeSinceEpoch);
    }
    std::chrono::milliseconds Timer::startTimeMilliseconds_ = millisecondsSinceEpoch();
    std::chrono::nanoseconds Timer::startTimeNanoseconds_ = nanosecondsSinceEpoch();

    Timer::Timer()
    {
    }

    Timer::~Timer()
    {
    }

    unsigned int Timer::milliseconds()
    {
        auto tmp = (millisecondsSinceEpoch() - Timer::startTimeMilliseconds_).count();
        return (unsigned int)tmp;
    }

    uint64_t Timer::nanoseconds()
    {
        auto tmp = (nanosecondsSinceEpoch() - Timer::startTimeNanoseconds_).count();
        return (uint64_t)tmp;
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

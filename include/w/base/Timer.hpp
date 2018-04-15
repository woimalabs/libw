/**
 * libw
 *
 * Copyright (C) 2012-2015 Woima Solutions
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

#ifndef LIBW_TIMER
#define LIBW_TIMER

#include <time.h>
#include <stdint.h> // For uint64_t
#include <chrono>

namespace w
{
    class Timer
    {
    public:
        Timer();
        ~Timer();

        /**
         * @return milliseconds. NOTE: start time undefined. Values
         *     are useful only for example when looking time differences.
         */
        static unsigned int milliseconds();

        /**
         * @return nanoseconds. NOTE: start time undefined. Values
         *     are useful only for example when looking time differences.
         */
        static uint64_t nanoseconds();

        static void sleepMilliseconds(unsigned int milliseconds);

    private:
        static void nanoSleepFromMilliseconds(unsigned int milliseconds);
        static std::chrono::milliseconds startTimeMilliseconds_;
        static std::chrono::nanoseconds startTimeNanoseconds_;
    };
}

#endif

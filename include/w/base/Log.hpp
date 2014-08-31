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

#ifndef LIBW_BASE_LOG
#define LIBW_BASE_LOG

#ifdef __APPLE__
#include <stdio.h>
#else
#include <cstdio>
#endif

#define DEBUG 1

#define LOG w::Log::D("%s:%s:%d", __FILE__, __FUNCTION__, __LINE__);
#define LOGD(...) w::Log::D(__VA_ARGS__);
#define LOGI(...) w::Log::I(__VA_ARGS__);
#define LOGE(...) w::Log::E(__VA_ARGS__);

namespace w
{
    class Log
    {
    public:
#ifdef DEBUG
        static void D(const char *fmt, ...);
#else
        static void D(const char *fmt, ...) {}
#endif
        static void I(const char *fmt, ...);
        static void E(const char *fmt, ...);
    };
}

#endif

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

#include "w/base/Log.hpp"
#include <cstdio>
#ifdef __APPLE__
    #include <stdarg.h>
#else
    #include <cstdarg>
#endif
#include <string>
#include <sstream>
#ifndef __ANDROID__
    #include <unistd.h>
#endif

// Continuation of a previous log message on the same line.
#define LOG_CONTINUE "\x10"

namespace w
{
    #ifdef __ANDROID__
        #include <android/log.h>
        #define ANDROID_LOG_TAG "libw"
    #endif

    #ifndef __ANDROID__
        #ifdef __APPLE__
            static const char *terminalColorNormal ("");
            static const char *terminalColorRed ("");
            static const char *terminalColorBlue ("");
            static const char *terminalColorBrown ("");
        #else // LINUX
            static const char *terminalColorNormal ("\033[0m");
            static const char *terminalColorRed ("\033[1;31m");
            static const char *terminalColorBlue ("\033[34m");
            static const char *terminalColorBrown ("\033[33m");
        #endif

        static void prefixMessage(FILE *stream, const char *color,
            const char *prefix, const char *fmt, va_list ap)
        {
            va_list aq;

            // Estimate message size
            va_copy (aq, ap);
            int msg_size = vsnprintf(NULL, 0, fmt, aq);
            va_end (aq);

            // Buffer for the message
            char *buf = new char[msg_size + 1];

            // Store the message in the buffer
            va_copy (aq, ap);
            vsnprintf (buf, msg_size + 1, fmt, aq);
            va_end (aq);

            // Print the message lines prefixed with the supplied prefix.
            // If the target stream is a terminal make the prefix colored.
            bool useColor = isatty (fileno(stream));
            const char *startColor (useColor ? color : "");
            const char *endColor (useColor && *color ? terminalColorNormal : "");

            std::string line;
            std::stringstream ss(buf);

            while (std::getline(ss, line))
            {
                // If line is a continuation of a previous message print the line plainly
                if (line[0] == LOG_CONTINUE[0])
                {
                    fprintf (stream, "%s", line.c_str() + 1);
                }
                else
                {
                    // Normal line, emit the prefix
                    if (prefix != NULL)
                    {
                        fprintf (stream, "%s%s%s: %s", startColor, prefix, endColor, line.c_str());
                    }
                    else
                    {
                        fprintf (stream, "%s", line.c_str());
                    }
                }

                if (!(ss.rdstate()))
                {
                    fputs ("\n", stream);
                }
            }
            fputs ("\n", stream);

            delete[] buf;
        }

        #ifdef DEBUG
            void Log::D(const char *fmt, ...)
            {
                va_list ap;
                va_start (ap, fmt);
                prefixMessage (stdout, terminalColorBrown, "DEBUG", fmt, ap);
                va_end (ap);
            }
        #endif

        void Log::I(const char *fmt, ...)
        {
            va_list ap;
            va_start (ap, fmt);
            prefixMessage (stdout, terminalColorBlue, "INFO", fmt, ap);
            va_end (ap);
        }

        void Log::E(const char *fmt, ...)
        {
            va_list ap;
            va_start (ap, fmt);
            prefixMessage (stderr, terminalColorRed, "ERROR", fmt, ap);
            va_end (ap);
        }
    #endif
    #ifdef __ANDROID__
        #ifdef DEBUG
            void Log::D(const char *fmt, ...)
            {
                va_list ap;
                va_start(ap, fmt);
                __android_log_vprint(ANDROID_LOG_DEBUG, ANDROID_LOG_TAG, fmt, ap);
                va_end(ap);
            }
        #endif

        void Log::I(const char *fmt, ...)
        {
            va_list ap;
            va_start(ap, fmt);
            __android_log_vprint(ANDROID_LOG_INFO, ANDROID_LOG_TAG, fmt, ap);
            va_end(ap);
        }

        void Log::E(const char *fmt, ...)
        {
            va_list ap;
            va_start(ap, fmt);
            __android_log_vprint(ANDROID_LOG_ERROR, ANDROID_LOG_TAG, fmt, ap);
            va_end(ap);
        }
    #endif
}

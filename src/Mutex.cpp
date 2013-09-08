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

#include "Mutex.hpp"
#include "w/Log.hpp"
#include "w/Exception.hpp"
#include <stdlib.h>
#include <errno.h>

namespace w
{
    Mutex::Mutex()
    {
        int r = pthread_mutex_init(&mutex_, NULL);

        if (r == EINVAL)
        {
            throw Exception("Mutex(), errno:EAGAIN");
        }
        else if (r == ENOMEM)
        {
            throw Exception("Mutex(), errno:ENOMEM");
        }
        else if (r == EPERM)
        {
            throw Exception("Mutex(), errno:EPERM");
        }
    }

    Mutex::~Mutex()
    {
        int r = 0;

        r = pthread_mutex_destroy(&mutex_);
        if (r == EINVAL)
        {
            throw Exception("~Mutex, errno:EINVAL");
        }
        else if (r == EBUSY)
        {
            throw Exception("~Mutex, errno:EBUSY");
        }
    }

    int Mutex::lock()
    {
        return pthread_mutex_lock(&mutex_);
    }

    int Mutex::tryLock()
    {
        return pthread_mutex_trylock(&mutex_);
    }

    int Mutex::unlock()
    {
        return pthread_mutex_unlock(&mutex_);
    }
}

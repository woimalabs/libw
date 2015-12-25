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

#include "w/base/Condition.hpp"
#include "w/base/Exception.hpp"
#include "w/base/Log.hpp"
#include <errno.h>

namespace w
{
    Condition::Condition()
    {
        int tmp = pthread_cond_init(&condition_, NULL);
        if (tmp == EAGAIN)
        {
            throw Exception("Condition::init, errno:EAGAIN");
        }
        else if (tmp == ENOMEM)
        {
            throw Exception("Condition::init, errno:ENOMEM");
        }
        else if (tmp == EBUSY)
        {
            throw Exception("Condition::init, errno:EBUSY");
        }
        else if (tmp == EINVAL)
        {
            throw Exception("Condition::init, errno:EINVAL");
        }
    }

    Condition::~Condition()
    {
        int tmp = pthread_cond_destroy(&condition_);
        if (tmp == EBUSY)
        {
            LOGE("Condition::~Condition, errno:EBUSY. Continue without exception.");
        }
        else if (tmp == ENOMEM)
        {
            LOGE("Condition::~Condition, errno:ENOMEM. Continue without exception.");
        }
    }
    
    int Condition::wait(Mutex &mutex)
    {
        return pthread_cond_wait(&condition_, &(mutex.mutex_));
    }
    
    void Condition::signal()
    {
        if (pthread_cond_signal(&condition_) == EINVAL)
        {
            throw Exception("Condition::signal, EINVAL");
        }
    }
}

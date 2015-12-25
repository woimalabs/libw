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

#include "w/base/Thread.hpp"
#include "w/base/Log.hpp"
#include "w/base/Exception.hpp"
#include <stdlib.h>
#include <errno.h>
#include <stdexcept>

namespace w
{
    void* threadFunction_(void* instance)
    {
        try
        {
            #ifdef __APPLE__
                NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init]; // Top-level pool
            #endif
            
            Thread* tmp = (Thread*)instance;
            ThreadCppWrapper::protectedCppRun(tmp);
            
            #ifdef __APPLE__
                [pool release];  // Release the objects in the pool.
            #endif

        }
        catch (Exception const& e)
        {
            LOGE("Exception inside thread! &s\n", e.what());
        }
        catch (...)
        {
            LOGE("Exception inside thread!\n");
        }

        return NULL;
    }

    Thread::Thread()
    {
    }

    Thread::~Thread()
    {
    }

    void Thread::start()
    {
        pthread_attr_t type;
        if (pthread_attr_init(&type) != 0)
        {
            throw Exception("pthread_attr_init failed");
        }
        pthread_attr_setdetachstate(&type, PTHREAD_CREATE_DETACHED);

        int result = pthread_create(&thread_, &type, threadFunction_, (void*)this);
        if (result == EAGAIN)
        {
            throw Exception("Thread::start, errno:EAGAIN");
        }
        else if (result == EINVAL)
        {
            throw Exception("Thread::start, errno:EINVAL");
        }
        else if (result == EPERM)
        {
            throw Exception("Thread::start, errno:EPERM");
        }
    }

    /*void Thread::join()
    {
        int result = pthread_join(thread_, NULL);
        if (result == EINVAL)
        {
            throw Exception("Thread::join, errno:EINVAL");
        }
        else if (result == ESRCH)
        {
            throw Exception("Thread::join, errno:ESRCH");
        }
        else if (result == EDEADLK)
        {
            throw Exception("Thread::join, errno:EDEADLK");
        }
    }*/
}

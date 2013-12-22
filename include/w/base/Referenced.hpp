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

#ifndef LIBW_BASE_REFERENCED
#define LIBW_BASE_REFERENCED

#include "w/base/Lock.hpp"
#include <cstddef>
using std::ptrdiff_t; // NOTE: sigc++ needs this line
#include <sigc++/signal.h>
#include "w/base/Log.hpp"

namespace w
{
    class Referenced
    {
    public:
        sigc::signal<void, int> destroy;

        unsigned int increment()
        {
            referenceCount_++;
            return referenceCount_;
        }

        unsigned int decrement()
        {
            referenceCount_--;
            if (referenceCount_ <= 0)
            {
                destroy.emit(id_);
                delete this;
            }
            return referenceCount_;
        }

        unsigned int referenceCount()
        {
            return referenceCount_;
        }

        unsigned int id()
        {
            return id_;
        }

    protected:
        Referenced():
            referenceCount_(0),
            id_(0)
        {
            Lock lock(mutex_);
            id_ = ++lastId_;
        }

        virtual ~Referenced()
        {
        }

    private:
        unsigned int referenceCount_;
        unsigned int id_;

        static Mutex mutex_;
        static unsigned int lastId_;
    };
}

#endif

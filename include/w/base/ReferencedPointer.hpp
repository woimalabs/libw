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

#ifndef LIBW_REFERENCEDPOINTER
#define LIBW_REFERENCEDPOINTER

#include "w/base/Referenced.hpp"
#include "w/base/Exception.hpp"

namespace w
{
    /**
    * @class ReferencedPointer
    *
    * Class to hold reference from classes that are inherited from Referenced class
    */
    template <class T> class ReferencedPointer
    {
    public:
        ReferencedPointer():
            instance_(NULL)
        {
        }

        ReferencedPointer(T* instance):
            instance_(instance)
        {
            if (instance_ != NULL)
            {
                Referenced* tmp = dynamic_cast<Referenced*>(instance);
                if (tmp == NULL)
                {
                    throw Exception("ReferencedPointer can hold only Referenced classes.");
                }
                instance_->increment();
            }
        }

        ~ReferencedPointer()
        {
            if (instance_ != NULL)
            {
                instance_->decrement();
                instance_ = NULL;
            }
        }

        ReferencedPointer& operator=(ReferencedPointer const& r)
        {
            if (this != &r)
            {
                if (instance_ != r.instance_)
                {
                    if (instance_ != NULL)
                    {
                        instance_->decrement();
                        instance_ = NULL;
                    }
                }

                // Assign r instance if it's other than NULL. NULL is our initial value.
                if (r.instance_ != NULL)
                {
                    instance_ = r.instance_;
                    instance_->increment();
                }
            }
            return *this;
        }

        ReferencedPointer(ReferencedPointer const& r)
        {
            instance_ = r.instance_;
            if (instance_ != NULL)
            {
                instance_->increment();
            }
        }

        bool isNull() const
        {
            return (instance_ == NULL) ? true : false;
        }

        bool exists() const
        {
            return (instance_ != NULL) ? true : false;
        }

        T* pointer() const
        {
            return instance_;
        }

    private:
        T* instance_;
    };
}

#endif

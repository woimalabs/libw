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

#ifndef LIBW_EXCEPTION
#define LIBW_EXCEPTION

#include <stdexcept>

namespace w
{
    class Exception: public std::exception
    {
    public:
        explicit Exception(const char* message):
            std::exception(),
            message_(message)
        {
        }

        virtual ~Exception() throw()
        {
        }

        virtual void raise()
        {
            throw *this;
        }

        virtual const char* what() const throw()
        {
            return message_;
        }

        Exception(Exception const& other):
            message_(other.message_)
        {
        }

    private:
        const char* message_;
    };
}

#endif

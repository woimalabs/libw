/**
 * libw
 *
 * Copyright (C) 2013-2015 Woima Solutions
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

#include "w/network/Request.hpp"

namespace w
{
    namespace network
    {
        Request::Request(std::string const& URL, unsigned int port):
            Referenced(),
            URL_(URL),
            port_(port),
            response_(ReferencedPointer<Response>(new Response))
        {
        }

        Request::~Request()
        {
        }

        void Request::send()
        {
        }

        void Request::addRawHeader(std::string const& key, std::string const& value)
        {
            // todo
        }

        ReferencedPointer<Response> Request::response()
        {
            return response_;
        }
    }
}

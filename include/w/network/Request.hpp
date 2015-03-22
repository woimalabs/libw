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

#ifndef LIBW_NETWORK_REQUEST
#define LIBW_NETWORK_REQUEST

#include "w/base/Referenced.hpp"
#include "w/network/Response.hpp"
#include "w/base/ReferencedPointer.hpp"
#include "w/base/Thread.hpp"
#include <string>

namespace w
{
    namespace network
    {
        class Request: public Referenced, public Thread
        {
        public:
            /**
             * Creates a network request class.
             */
            Request(std::string const& url, unsigned int port = 80);
            ~Request();
            void send();
            void addRawHeader(std::string const& key, std::string const& value);
            ReferencedPointer<Response> response();
            void threadFunction();

        private:
            std::string URL_;
            unsigned int port_;
            ReferencedPointer<Response> response_;
        };
    }
}

#endif

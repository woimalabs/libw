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

#include "w/events/EventBuffer.hpp"
#include <w/base/Lock.hpp>
#include <w/graphics/Window.hpp>

namespace w
{
    #if __ANDROID__
        EventBuffer::EventBuffer(const Window& window)
    #elif __linux__
        EventBuffer::EventBuffer(const Window& window):
            xDisplay_(window.xDisplay())
    #elif __APPLE__
        EventBuffer::EventBuffer(const Window& window)
    #endif
    {
    }

    void EventBuffer::add(Event* event)
    {
        LOCK

        events_.push_back(event);
    }

    Event* EventBuffer::pop()
    {
        LOCK

        Event* r = NULL;
        if (events_.empty() == false)
        {
            r = events_.front();
            events_.pop_front();
        }

        return r;
    }
}

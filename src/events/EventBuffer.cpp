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

#include "EventBufferPrivate.hpp"
#include "w/events/EventBuffer.hpp"
#include <w/base/Log.hpp>
#include <w/base/Lock.hpp>
#include <w/graphics/Window.hpp>

namespace w
{
    namespace events
    {
        EventBuffer::EventBuffer(const graphics::Window& window):
            private_(new EventBufferPrivate(window))
        {
            private_->increment();
        }

        EventBuffer::EventBuffer(EventBuffer const& r):
            private_(r.private_)
        {
            private_->increment();
        }

        EventBuffer::~EventBuffer()
        {
            private_->decrement();
        }

        EventBuffer& EventBuffer::operator=(EventBuffer const& r)
        {
            if (this != &r)
            {
                private_ = r.private_;
                private_->increment();
            }
            return *this;
        }

        void EventBuffer::add(Event* event)
        {
            private_->add(event);
        }

        Event* EventBuffer::pop()
        {
            return private_->pop();
        }
    }
}

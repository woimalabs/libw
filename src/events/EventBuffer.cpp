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

    void EventBuffer::pollXEvent()
    {
        if (xDisplay_ == NULL)
        {
            return;
        }
        else if (!XPending(xDisplay_))
        {
            return;
        }

        XEvent xEvent;
        XNextEvent(xDisplay_, &xEvent);
        Event* event = new Event;

        if (xEvent.type == KeyPress)
        {
            event->type = EventType::Keyboard;
            if (XLookupKeysym(&xEvent.xkey, 0) == XK_Escape)
            {
                event->keyboard.symbol = InputKey::Escape;
            }
            else if (XLookupKeysym(&xEvent.xkey, 0) == XK_Up)
            {
                event->keyboard.symbol = InputKey::ArrowUp;
            }
            else if (XLookupKeysym(&xEvent.xkey, 0) == XK_Down)
            {
                event->keyboard.symbol = InputKey::ArrowDown;
            }
            else if (XLookupKeysym(&xEvent.xkey, 0) == XK_Left)
            {
                event->keyboard.symbol = InputKey::ArrowLeft;
            }
            else if (XLookupKeysym(&xEvent.xkey, 0) == XK_Right)
            {
                event->keyboard.symbol = InputKey::ArrowRight;
            }
        }
        else if (xEvent.type == ClientMessage)
        {

        }
        else if (xEvent.type == MotionNotify || xEvent.type == ButtonPress || xEvent.type == ButtonRelease)
        {
            static bool lastTouchAvailable = false;
            static unsigned int lastX = 0;
            static unsigned int lastY = 0;

            event->type = EventType::Touch;
            event->touch.id = 0;
            event->touch.x = xEvent.xmotion.x;
            event->touch.y = xEvent.xmotion.y;
            if (lastTouchAvailable == true)
            {
                event->touch.lastX = lastX;
                event->touch.lastY = lastY;
            }

            unsigned int touchFlags;
            if (xEvent.type == MotionNotify)
            {
                touchFlags += TouchFlags::Moved;
            }
            if (xEvent.type == ButtonPress)
            {
                touchFlags += TouchFlags::Pressed;
            }
            if (xEvent.type == ButtonRelease)
            {
                touchFlags += TouchFlags::Released;
            }
            if (lastX == xEvent.xmotion.x && lastY == xEvent.xmotion.y)
            {
                touchFlags += TouchFlags::Stationary;
            }
            event->touch.flags = touchFlags;

            lastX = xEvent.xmotion.x;
            lastY = xEvent.xmotion.y;
            lastTouchAvailable = true;
        }

        add(event);
    }
}

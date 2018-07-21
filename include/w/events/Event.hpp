/**
 * libw
 *
 * Copyright (C) 2013-2016 Woima Solutions
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

#ifndef LIBW_EVENTS_EVENT
#define LIBW_EVENTS_EVENT

namespace w
{
    namespace events
    {
        struct EventType
        {
            enum Enum
            {
                System,
                Touch,
                Keyboard,
                Unknown
            };
        };

        /// System

        struct SystemFlags
        {
            enum Enum
            {
                Closed = 0x01
                // TODO
            };
        };

        struct SystemEvent
        {
            EventType::Enum type;
            unsigned int flags;
        };

        /// Touch

        struct TouchFlags
        {
            enum Enum
            {
                Pressed = 0x01,
                Moved = 0x02,
                Stationary = 0x04,
                Released = 0x08
            };
        };

        struct TouchEvent
        {
            EventType::Enum type;
            unsigned int id;
            float x;
            float y;
            float lastX;
            float lastY;
            unsigned int flags;
            unsigned int mouseButtonNumber;
        };

        /// Keyboard

        struct KeyboardSymbol
        {
            enum Enum
            {
                Escape,
                ArrowUp,
                ArrowDown,
                ArrowRight,
                ArrowLeft,
                p,
                w,
                a,
                s,
                d
            };
        };

        struct KeyboardEvent
        {
            EventType::Enum type;

            bool keydown; // For caps & numlock: set when 1st pressed, unset when pressed again
            bool pressed;
            KeyboardSymbol::Enum symbol;
        };

        union Event
        {
            EventType::Enum type;
            bool handled;
            SystemEvent system;
            TouchEvent touch;
            KeyboardEvent keyboard;
        };
    }
}

#endif


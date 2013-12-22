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

#include "AppleTouchTracker.hpp"
#include <w/base/Log.hpp>

namespace w
{
    int AppleTouchTracker::getId(void* touch)
    {
        for (int i = 0; i < AppleTouchTracker::MaxTouches; i++)
        {
            if (touches_[i] == touch)
            {
                return i;
            }
        }
        
        //LOGD("Can't locate fingerID by touch %d", touch);
        return -1;
    }

    int AppleTouchTracker::addNewTouch(void* touch)
    {
        for (int i = 0; i < AppleTouchTracker::MaxTouches; i++)
        {
            if (touches_[i] == NULL)
            {
                // Empty slot
                touches_[i] = touch;
                return i;
            }
        }
        
        LOGE("Can't add new touch id: %d", touch);
        return -1;
    }
        
    int AppleTouchTracker::endTouch(void* touch)
    {
        for (int i = 0; i < AppleTouchTracker::MaxTouches; i++)
        {
            if (touches_[i] == touch)
            {
                touches_[i] = NULL;
                return i;
            }
        }
            
        LOGE("Can't end touch id: %d", touch);
        return -1;
    }

    int AppleTouchTracker::getTouchesActive()
    {
        int r = 0;
        
        for (int i = 0; i < AppleTouchTracker::MaxTouches; i++)
        {
            if (touches_[i] != NULL)
            {
                r++;
            }
        }
        
        return r;
    }
}

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

#include "w/animation/AbstractAnimation.hpp"
#include "AnimationEnginePrivate.hpp"
#include "w/base/Exception.hpp"
#include "w/base/Log.hpp"
#include "w/base/Timer.hpp"

namespace w
{
    namespace animation
    {
        AbstractAnimation::AbstractAnimation(float millisecondLength, bool loop):
            millisecondLength_(millisecondLength),
            millisecondStart_(0.0f),
            started_(false),
            loop_(loop)
        {
        }

        AbstractAnimation::~AbstractAnimation()
        {
        }

        void AbstractAnimation::start()
        {
            started_ = true;
            millisecondStart_ = AnimationEnginePrivate::currentTimeMilliseconds();
        }

        float AbstractAnimation::progress()
        {
            float tmp = 0.0f;
            if(started_ == true)
            {
                tmp = AnimationEnginePrivate::currentTimeMilliseconds() - millisecondStart_;
                if(tmp > millisecondLength_)
                {
                    if(loop_ == true)
                    {
                        tmp -= millisecondLength_;
                        millisecondStart_ += millisecondLength_;
                    }
                    else
                    {
                        tmp = millisecondLength_;
                    }
                }
            }

            return tmp / millisecondLength_;
        }
    }
}

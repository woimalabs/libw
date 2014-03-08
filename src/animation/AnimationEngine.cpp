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

#include "w/animation/AnimationEngine.hpp"
#include "AnimationEnginePrivate.hpp"
#include <w/base/Log.hpp>
#include <w/base/Exception.hpp>

namespace w
{
    namespace animation
    {
        AnimationEngine::AnimationEngine()
        {
            private_ = new AnimationEnginePrivate();
        }

        AnimationEngine::~AnimationEngine()
        {
            LOGI("Shutting down AnimationEngine...")
            if (private_ != NULL)
            {
                delete private_;
                private_ = NULL;
            }
            else
            {
                LOGE("AnimationEngine::~AnimationEngine(), private == NULL, AnimationEngine in corrupted state!")
            }
            LOGI("Shutted down AnimationEngine.")
        }

        void AnimationEngine::stepMilliseconds(float value)
        {
            if (private_ != NULL)
            {
                return private_->stepMilliseconds(value);
            }
            else
            {
                throw Exception("AnimationEngine::volume(), AnimationEngine does not exist.");
            }
        }
    }
}

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

#ifndef LIBW_ANIMATION_ABSTRACTANIMATION
#define LIBW_ANIMATION_ABSTRACTANIMATION

#include "w/base/Class.hpp"
#include "w/base/Referenced.hpp"

namespace w
{
    namespace animation
    {
        /**
        * @class AbstractAnimation
        *
        * Base class for animations.
        *
        * @note     AnimationEngine must exists during lifetime of AbstractAnimation
        *           instances to work.
        */
        class AbstractAnimation: public Referenced
        {
        public:
            UNCOPYABLE(AbstractAnimation)

            virtual ~AbstractAnimation();
            void start();
            float progress() const;

        protected:
            /**
             * Base class for animations.
             *
             * @param [in] millisecondLength        Animation length in milliseconds
             * @param [in] isLooping                Does animation loop: true for yes, false for no
             */
            AbstractAnimation(float millisecondLength, bool isLooping = true);

        private:
            float millisecondLength_;
            float millisecondStart_;
            bool started_;
            bool loop_;
        };
    }
}

#endif

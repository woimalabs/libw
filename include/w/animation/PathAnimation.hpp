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

#ifndef LIBW_ANIMATION_PATHANIMATION
#define LIBW_ANIMATION_PATHANIMATION

#include "w/animation/AbstractAnimation.hpp"
#include "w/animation/ControlPoint.hpp"
#include "w/base/Class.hpp"
#include "w/base/ReferencedPointer.hpp"
#include <vector>

namespace w
{
    namespace animation
    {
        /**
        * @class PathAnimation
        *
        * PathAnimation with possibility to affect to next variables:
        *   -
        *
        * @note     AnimationEngine must exists during lifetime of AbstractAnimation
        *           instances to work.
        */
        class PathAnimation: public AbstractAnimation
        {
        public:
            UNCOPYABLE(PathAnimation)

            PathAnimation(std::vector<w::ReferencedPointer<w::animation::ControlPoint> > & points,
                float millisecondLength,
                bool loop = true);

            Eigen::Vector3f location();
            Eigen::Matrix4f rotation();
            Eigen::Vector3f scale();
            float opacity();

        private:
            inline unsigned int progressIndex();
            inline float progressOverTheIndex();
            inline unsigned int nextIndex(unsigned int currentIndex);
            const std::vector<w::ReferencedPointer<w::animation::ControlPoint> > points_;
            float progressPerPoint_;
        };
    }
}

#endif

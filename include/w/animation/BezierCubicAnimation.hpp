/**
 * libw
 *
 * Copyright (C) 2013-2014 Woima Solutions
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

#ifndef LIBW_ANIMATION_BEZIERCUBICANIMATION
#define LIBW_ANIMATION_BEZIERCUBICANIMATION

#include "w/animation/BezierCubicAnimation.hpp"
#include "w/animation/PathAnimation.hpp"
#include "w/base/Class.hpp"
#include "w/base/ReferencedPointer.hpp"
#include <vector>

namespace w
{
    namespace animation
    {
        /**
        * @class BezierCubicAnimation
        *
        * BezierCubicAnimation to animate through series of given control points against time.
        *
        * f(t)
        * =   [(1 - t)^3] *p0
        *   + [3 * (1 - t)^2 * t] *p1
        *   + [3 * (1 - t) * t^2] *p2
        *   + [t^3] *p3
        */
        class BezierCubicAnimation: public PathAnimation
        {
        public:
            UNCOPYABLE(BezierCubicAnimation)

            /**
             * @param points    Four points should be given
             */
            BezierCubicAnimation(std::vector<w::ReferencedPointer<w::animation::ControlPoint> > & points,
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
            float progressPerSegment_;
        };
    }
}

#endif

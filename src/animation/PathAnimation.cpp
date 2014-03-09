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

#include "w/animation/PathAnimation.hpp"
#include "w/math/Math.hpp"

namespace w
{
    namespace animation
    {
        PathAnimation::PathAnimation(const std::vector<ReferencedPointer<w::animation::ControlPoint> > & points,
            float millisecondLength,
            bool loop):

            AbstractAnimation(millisecondLength, loop),
            points_(points),
            progressPerPoint_(1.0f / (float)points_.size())
        {
        }

        Eigen::Vector3f PathAnimation::location()
        {
            unsigned int i = progressIndex();
            unsigned int iNext = i + 1;
            return points_[i].pointer()->location() +
                (progressOverTheIndex() * points_[iNext].pointer()->location());
        }

        Eigen::Matrix4f PathAnimation::rotation()
        {
            unsigned int i = progressIndex();
            unsigned int iNext = i + 1;
            return points_[i].pointer()->rotation() +
                (progressOverTheIndex() * points_[iNext].pointer()->rotation());
        }

        Eigen::Vector3f PathAnimation::scale()
        {
            unsigned int i = progressIndex();
            unsigned int iNext = i + 1;
            return points_[i].pointer()->scale() +
                (progressOverTheIndex() * points_[iNext].pointer()->scale());
        }

        float PathAnimation::opacity()
        {
            unsigned int i = progressIndex();
            unsigned int iNext = i + 1;
            return points_[i].pointer()->opacity() +
                (progressOverTheIndex() * points_[iNext].pointer()->opacity());
        }

        inline unsigned int PathAnimation::progressIndex()
        {
            unsigned int r = progress() * points_.size();
            if(r == points_.size())
            {
                r--;
            }
            return r;
        }

        inline float PathAnimation::progressOverTheIndex()
        {
            return progress() - (progressIndex() * progressPerPoint_);
        }
    }
}


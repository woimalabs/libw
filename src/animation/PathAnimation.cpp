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
        PathAnimation::PathAnimation(std::vector<ReferencedPointer<w::animation::ControlPoint> > & points,
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
            unsigned int iNext = nextIndex(i);
            float iNextFactor = progressOverTheIndex() / progressPerPoint_;
            float iFactor = 1.0f - iNextFactor;
            return iFactor * points_[i].pointer()->location()
                + iNextFactor * points_[iNext].pointer()->location();
        }

        Eigen::Matrix4f PathAnimation::rotation()
        {
            unsigned int i = progressIndex();
            unsigned int iNext = nextIndex(i);
            Eigen::Quaternionf qI(points_[i].pointer()->rotation());
            Eigen::Quaternionf qINext(points_[iNext].pointer()->rotation());
            Eigen::Quaternionf tmp(qI.slerp(progressOverTheIndex()/progressPerPoint_, qINext));
            return Eigen::Affine3f(tmp).matrix();
        }

        Eigen::Vector3f PathAnimation::scale()
        {
            unsigned int i = progressIndex();
            unsigned int iNext = nextIndex(i);
            float iNextFactor = progressOverTheIndex() / progressPerPoint_;
            float iFactor = 1.0f - iNextFactor;
            return iFactor * points_[i].pointer()->scale()
                + iNextFactor * points_[iNext].pointer()->scale();
        }

        float PathAnimation::opacity()
        {
            unsigned int i = progressIndex();
            unsigned int iNext = nextIndex(i);
            return points_[i].pointer()->opacity() +
                (progressOverTheIndex() * points_[iNext].pointer()->opacity());
        }

        inline unsigned int PathAnimation::progressIndex()
        {
            unsigned int r = progress() * (float)points_.size();
            if(r != 0 && r >= points_.size())
            {
                r = points_.size() - 1;
            }
            return r;
        }

        inline float PathAnimation::progressOverTheIndex()
        {
            return progress() - (progressIndex() * progressPerPoint_);
        }

        inline unsigned int PathAnimation::nextIndex(unsigned int currentIndex)
        {
            unsigned int r = currentIndex + 1;
            if(r >= points_.size())
            {
                r = 0;
            }
            return r;
        }
    }
}


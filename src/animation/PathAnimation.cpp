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

#include "w/animation/PathAnimation.hpp"
#include "w/math/Math.hpp"
#include "w/base/Exception.hpp"

namespace w
{
    namespace animation
    {
        /**
         * @param   t   current time
         * @param   b   start value
         * @param   c   change in value
         * @param   d   duration time
         */
        float easeInOutQuint(float t, float b, float c, float d)
        {
            t /= d/2.0f;
            if (t < 1) return c/2*t*t*t*t*t + b;
            t -= 2.0f;
            return c/2.0f*(t*t*t*t*t + 2.0f) + b;
        };
        
        /**
         * @param   t   current time
         * @param   b   start value
         * @param   c   change in value
         * @param   d   duration time
         */
        float easeInQuint(float t, float b, float c, float d)
        {
            t /= d;
            return c*t*t*t*t*t + b;
        };
        
        /**
         * @param   t   current time
         * @param   b   start value
         * @param   c   change in value
         * @param   d   duration time
         */
        float easeOutQuint(float t, float b, float c, float d)
        {
            t /= d;
            t--;
            return c*(t*t*t*t*t + 1) + b;
        };
        
        /**
         * @param   t   current time from 0.0f to 1.0f
         */
        float easeOutElastic(float t)
        {
            float p = 0.3f;
            float r = pow(2.0f, -10.0f*t) * sin((t - p/4.0f)*(2.0f * M_PI)/p) + 1.0f;
            return r;
        }

        PathAnimation::PathAnimation(std::vector<ReferencedPointer<w::animation::ControlPoint> > & points,
            float millisecondLength,
            bool loop,
            Easing::Enum easing):

            AbstractAnimation(millisecondLength, loop),
            points_(points),
            progressPerSegment_(1.0f / (float)(points_.size() - 1)),
            easing_(easing)
        {
            if(points_.size() == 0)
            {
                throw w::Exception("PathAnimation needs at least 1 point to be constructed!");
            }
        }

        PathAnimation::~PathAnimation()
        {
        }

        Eigen::Vector3f PathAnimation::location()
        {
            unsigned int i = progressIndex();
            unsigned int iNext = nextIndex(i);
            Eigen::Vector3f tmp = points_[iNext].pointer()->location() - points_[i].pointer()->location();
            if(i == iNext && iNext > 0) // at the end
            {
                tmp = points_[iNext].pointer()->location() - points_[iNext - 1].pointer()->location();
            }
            float iNextFactor = progressOverTheIndex() / progressPerSegment_;
            return points_[i].pointer()->location() + iNextFactor * tmp;
        }

        Eigen::Matrix4f PathAnimation::rotation()
        {
            unsigned int i = progressIndex();
            unsigned int iNext = nextIndex(i);
            Eigen::Quaternionf qI(points_[i].pointer()->rotation());
            if(iNext != i)
            {
                Eigen::Quaternionf qINext(points_[iNext].pointer()->rotation());
                float factor = progressOverTheIndex() / progressPerSegment_;
                Eigen::Quaternionf tmp(qI.slerp(factor, qINext));
                return Eigen::Affine3f(tmp).matrix();
            }

            // exactly at end
            return Eigen::Affine3f(qI).matrix();
        }

        Eigen::Vector3f PathAnimation::scale()
        {
            unsigned int i = progressIndex();
            unsigned int iNext = nextIndex(i);
            float iNextFactor = progressOverTheIndex() / progressPerSegment_;
            float iFactor = 1.0f - iNextFactor;
            return iFactor * points_[i].pointer()->scale()
                + iNextFactor * points_[iNext].pointer()->scale();
        }

        float PathAnimation::opacity()
        {
            unsigned int i = progressIndex();
            unsigned int iNext = nextIndex(i);
            float iNextFactor = progressOverTheIndex() / progressPerSegment_;
            float iFactor = 1.0f - iNextFactor;
            return iFactor * points_[i].pointer()->opacity()
                + iNextFactor * points_[iNext].pointer()->opacity();
        }

        const std::vector<w::ReferencedPointer<w::animation::ControlPoint> >& PathAnimation::points() const
        {
            return points_;
        }

        float PathAnimation::progress()
        {
            float progress = AbstractAnimation::progress();
            switch(easing_)
            {
                case Easing::Linear:
                {
                    return progress;
                    break;
                }
                case Easing::InOutQuint:
                {
                    return easeInOutQuint(progress, 0.0f, progress, 1.0f);
                    break;
                }
                case Easing::InQuint:
                {
                    return easeInQuint(progress, 0.0f, progress, 1.0f);
                    break;
                }
                case Easing::OutQuint:
                {
                    return easeOutQuint(progress, 0.0f, progress, 1.0f);
                    break;
                }
                case Easing::OutElastic:
                {
                    return easeOutElastic(progress);
                    break;
                }
            }

            // "backup" return value
            return progress;
        }

        unsigned int PathAnimation::progressIndex()
        {
            unsigned int r = progress() * (float)(points_.size() - 1);
            if(r >= points_.size() && r != 0)
            {
                r = points_.size() - 1;
            }
            return r;
        }

        float PathAnimation::progressOverTheIndex()
        {
            return progress() - (progressIndex() * progressPerSegment_);
        }

        unsigned int PathAnimation::nextIndex(unsigned int currentIndex)
        {
            unsigned int r = currentIndex + 1;
            if(r >= points_.size() && r != 0)
            {
                r = points_.size() - 1;
            }
            return r;
        }

        float PathAnimation::progressPerSegment() const
        {
            return progressPerSegment_;
        }
    }
}


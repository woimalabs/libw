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

#include "w/animation/BezierCubicAnimation.hpp"
#include "w/math/Math.hpp"
#include "w/base/Exception.hpp"
#include "w/base/Helpers.hpp"

namespace w
{
    namespace animation
    {
        BezierCubicAnimation::BezierCubicAnimation(std::vector<ReferencedPointer<w::animation::ControlPoint> > & points,
            float millisecondLength,
            bool loop):

            PathAnimation(points, millisecondLength, loop)
        {
            if(points.size() != 4)
            {
                throw w::Exception("BezierCubicAnimation must have exactly 4 points.");
            }
        }

        Eigen::Vector3f BezierCubicAnimation::location()
        {
            float t = w::clamp(progress(), 0.0f, 1.0f);
            float a = (1 - t);
            ControlPoint* p0 = points()[0].pointer();
            ControlPoint* p1 = points()[1].pointer();
            ControlPoint* p2 = points()[2].pointer();
            ControlPoint* p3 = points()[3].pointer();
            return (a*a*a)*p0->location() + (3*a*a*t)*p1->location() + (3*a*t*t)*p2->location() + (t*t*t)*p3->location();
        }

        Eigen::Matrix4f BezierCubicAnimation::rotation()
        {
            unsigned int i = progressIndex();
            unsigned int iNext = nextIndex(i);
            Eigen::Quaternionf qI(points()[i].pointer()->rotation());
            if(iNext != i)
            {
                Eigen::Quaternionf qINext(points()[iNext].pointer()->rotation());
                float factor = progressOverTheIndex() / progressPerSegment();
                Eigen::Quaternionf tmp(qI.slerp(factor, qINext));
                return Eigen::Affine3f(tmp).matrix();
            }

            // exactly at end
            return Eigen::Affine3f(qI).matrix();
        }

        Eigen::Vector3f BezierCubicAnimation::scale()
        {
            float t = w::clamp(progress(), 0.0f, 1.0f);
            float a = (1 - t);
            ControlPoint* p0 = points()[0].pointer();
            ControlPoint* p1 = points()[1].pointer();
            ControlPoint* p2 = points()[2].pointer();
            ControlPoint* p3 = points()[3].pointer();
            return (a*a*a)*p0->scale() + (3*a*a*t)*p1->scale() + (3*a*t*t)*p2->scale() + (t*t*t)*p3->scale();
        }

        float BezierCubicAnimation::opacity()
        {
            float t = w::clamp(progress(), 0.0f, 1.0f);
            float a = (1 - t);
            ControlPoint* p0 = points()[0].pointer();
            ControlPoint* p1 = points()[1].pointer();
            ControlPoint* p2 = points()[2].pointer();
            ControlPoint* p3 = points()[3].pointer();
            return (a*a*a)*p0->opacity() + (3*a*a*t)*p1->opacity() + (3*a*t*t)*p2->opacity() + (t*t*t)*p3->opacity();
        }
    }
}


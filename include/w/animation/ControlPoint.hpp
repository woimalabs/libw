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

#ifndef LIBW_ANIMATION_CONTROLPOINT
#define LIBW_ANIMATION_CONTROLPOINT

#include "w/base/Class.hpp"
#include "w/base/Referenced.hpp"
#include "w/math/Eigen.hpp"

namespace w
{
    namespace animation
    {
        /**
        * @class Point
        */
        class ControlPoint: public Referenced
        {
        public:
            UNCOPYABLE(ControlPoint)

            ControlPoint(
                const Eigen::Vector3f & location = Eigen::Vector3f::Zero(),
                const Eigen::Matrix4f & rotation = Eigen::Matrix4f::Identity(),
                const Eigen::Vector3f & scale = Eigen::Vector3f::Ones(),
                float opacity = 1.0f):

                location_(location),
                rotation_(rotation),
                scale_(scale),
                opacity_(opacity)
            {
            }

            Eigen::Vector3f location() const
            {
                return location_;
            }

            Eigen::Matrix4f rotation() const
            {
                return rotation_;
            }

            Eigen::Vector3f scale() const
            {
                return scale_;
            }

            float opacity() const
            {
                return opacity_;
            }

        private:
            Eigen::Vector3f location_;
            Eigen::Matrix4f rotation_;
            Eigen::Vector3f scale_;
            float opacity_;
        };
    }
}

#endif

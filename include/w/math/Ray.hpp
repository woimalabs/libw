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

#ifndef WORLD_MATH_RAY
#define WORLD_MATH_RAY

#include "w/scene/ComponentPrivate.hpp"
#include "w/graphics/Window.hpp"
#include <Eigen/Dense>

namespace w
{
    class Ray
    {
    public:
        Ray(const Eigen::Vector3f & location, const Eigen::Vector3f & directionUnit):
            location_(location),
            directionUnit_(directionUnit)
        {
        }

        Ray& operator=(Ray const& r)
        {
            if (this != &r)
            {
                location_ = r.location_;
                directionUnit_ = r.directionUnit_;
            }
            return *this;
        }

    private:
        Eigen::Vector3f location_;
        Eigen::Vector3f directionUnit_;
    };
}

#endif

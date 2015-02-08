
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
 * @author sami.enne@woimasolutions.com
 */

#ifndef LIBW_GRAPHICS_DISTANCE
#define LIBW_GRAPHICS_DISTANCE

#include "w/graphics/Aabb.hpp"
#include "w/graphics/Plane.hpp"

namespace w
{
    namespace graphics
    {
        inline float distance(Aabb const& aabb, Plane const& plane)
        {
            Eigen::Vector3f c = aabb.getCenter();
            Eigen::Vector3f e = aabb.getDelta() * 0.5;
            Eigen::Vector3f n(plane[0], plane[1], plane[2]);
            float d = plane[3];

            float r = e[0]*std::abs(n[0]) + e[1]*std::abs(n[1]) +
                e[2]*std::abs(n[2]);

            float s = c.dot(n) + d; // distance to box center

            if (std::abs(s) <= r) // Collision
                return 0.0;

            if (s < 0.0)
                return s+r;

            return s-r;
        }
    }
}

#endif


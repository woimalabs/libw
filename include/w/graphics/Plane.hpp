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

#ifndef LIBW_GRAPHICS_PLANE
#define LIBW_GRAPHICS_PLANE

#include <cmath>

namespace w
{
    namespace graphics
    {
        class Plane
        {
        public:
            inline Plane(float x, float y, float z, float d)
            {
                float len = std::sqrt(x*x+y*y+z*z);
                data_[0] = x / len;
                data_[1] = y / len;
                data_[2] = z / len;
                data_[3] = d / len;
            }

            inline ~Plane()
            {
            }

            inline Plane operator-() const
            {
                return Plane(-data_[0], -data_[1], -data_[2], -data_[3]);
            }

            inline float operator[](int index) const
            {
                return data_[index];
            }

            inline float const* data() const
            {
                return data_;
            }

        private:
            float data_[4];
        };
    }
}

#endif

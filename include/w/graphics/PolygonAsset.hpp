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

#ifndef LIBW_GRAPHICS_POLYGONASSET
#define LIBW_GRAPHICS_POLYGONASSET

#include <w/base/Class.hpp>
#include <w/math/Eigen.hpp>
#include <vector>

namespace w
{
    namespace graphics
    {
        class PolygonAsset
        {
        public:
            COPYABLE(PolygonAsset)

            struct Point
            {
                Point(float x, float y, float z):
                    x(x),
                    y(y),
                    z(z)
                {
                }

                float x, y, z;
            };

            PolygonAsset(const std::vector<PolygonAsset::Point> & data,
                const Eigen::Vector4f & color = Eigen::Vector4f(1.0f, 0.0f, 1.0f, 1.0f));
            ~PolygonAsset();
            Eigen::Vector4f color();

        private:
            friend class RendererPrivate;
            class PolygonAssetPrivate* private_;
        };
    }
}

#endif

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

#ifndef LIBW_GRAPHICS_AABB
#define LIBW_GRAPHICS_AABB

#include <Eigen/Geometry>

#include <ostream>

namespace w
{
    namespace graphics
    {
        class Aabb
        {
        public:
            Aabb();
            Aabb(Eigen::Vector4f const& v1, Eigen::Vector4f const& v2);
            Aabb(Eigen::Vector3f const& v1, Eigen::Vector3f const& v2);
            Aabb(Aabb const& other);
            Aabb const& operator=(Aabb const& other);
            ~Aabb();

            static Aabb fromData(void const* data, size_t size,
                    size_t offset, size_t stride);

            bool operator==(Aabb const& other) const;
            bool operator<(Aabb const& other) const;
            Aabb operator+(Aabb const& other) const;
            Aabb const& operator+=(Aabb const& aabb);

            Aabb const& operator+=(Eigen::Vector4f const& v);
            Aabb const& operator+=(Eigen::Vector3f const& v);

            Aabb operator+(Eigen::Vector4f const& v) const;
            Aabb operator+(Eigen::Vector3f const& v) const;
            Aabb operator*(Eigen::Matrix4f const& matrix) const;

            bool isEmpty() const;
            float getVolume() const;
            Eigen::Vector3f getMin() const;
            Eigen::Vector3f getMax() const;
            Eigen::Vector3f getCenter() const;
            Eigen::Vector3f getDelta() const;
            float getLongestSide() const;

            bool overlaps(Aabb const& other) const;
            bool contains(Eigen::Vector3f const& v) const;

        private:
            float x_;
            float y_;
            float z_;
            float dx_;
            float dy_;
            float dz_;
            bool empty_;
        };
    }
}

std::ostream& operator<<(std::ostream& stream, w::graphics::Aabb const& aabb);

#endif


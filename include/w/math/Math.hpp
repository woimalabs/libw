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

#ifndef LIBW_MATH_MATH
#define LIBW_MATH_MATH

#include <w/math/Eigen.hpp>

namespace w
{
    const float Pi = 3.1415;

    namespace math
    {
        static Eigen::Matrix4f lookAt(const Eigen::Vector3f& position, const Eigen::Vector3f& target, const Eigen::Vector3f& up)
        {
            Eigen::Matrix3f tmp;
            tmp.col(2) = (position-target).normalized();
            tmp.col(0) = up.cross(tmp.col(2)).normalized();
            tmp.col(1) = tmp.col(2).cross(tmp.col(0));

            Eigen::Matrix4f r;
            r.topLeftCorner<3, 3>() = tmp.transpose();
            r.topRightCorner<3, 1>() = -tmp.transpose() * position;
            return r;
        }

        static Eigen::Matrix4f view(const Eigen::Vector3f& location, const Eigen::Vector3f& forward, const Eigen::Vector3f& up)
        {
            return Eigen::Affine3f(Eigen::Translation3f(location)).matrix();

            // TODO
            //Eigen::Vector3f transformedForward = forward; //rotation.transformVector(forward, forward);
            //Eigen::Vector3f cameraLookAt = location + transformedForward;
            //return lookAt(location, cameraLookAt, Eigen::Vector3f(0.0f, 1.0f, 0.0f));
        }

        static Eigen::Matrix4f perspectiveProjection(float fovY, float aspect, float near, float far)
        {
            float theta = fovY * 0.5f;
            float range = far - near;
            float invtan = 1.0f / tan(theta);

            Eigen::Matrix4f r;
            r(0, 0) = invtan / aspect;
            r(1, 1) = invtan;
            r(2, 2) = -(near + far) / range;
            r(3, 2) = -1;
            r(2, 3) = -2 * near * far / range;
            r(3, 3) = 0;
            return r;
        }

        static Eigen::Matrix4f scale(float x, float y, float z)
        {
            Eigen::Matrix4f r = Eigen::Matrix4f::Identity();
            r(0, 0) = x;
            r(1, 1) = y;
            r(2, 2) = z;
            return r;
        }

        static Eigen::Matrix4f orthogonalProjection(float w, float h, float near, float far)
        {
            Eigen::Matrix4f r = Eigen::Matrix4f::Identity();

            r(0, 0) = 2.0f / w;
            r(1, 1) = 2.0f / h;
            r(2, 2) = 1.0f / (far - near);
            r(2, 3) = -near / (far - near);
            r(3, 3) = 1.0f;
            return r;
        }

        /**
         * Returns next power of two size for given number.
         */
        static unsigned int nextPowerOfTwo(unsigned int n)
        {
            n--;
            n |= n >> 1;
            n |= n >> 2;
            n |= n >> 4;
            n |= n >> 8;
            n |= n >> 16;
            n++;
            return n;
        }

        static float cross(const Eigen::Vector2f & a, const Eigen::Vector2f & b)
        {
            return a.x() * b.y() - a.y() * b.x();
        }

        static Eigen::Vector2f normal(const Eigen::Vector2f & line)
        {
            return Eigen::Vector2f(line.y(), -line.x());
        }

        /**
         * Returns minimum distance between line segment vw and point p
         */
        static float linePointDistance(const Eigen::Vector2f & v, const Eigen::Vector2f & w, const Eigen::Vector2f & p)
        {
            Eigen::Vector2f tmp(v - w);
            float l2 = tmp.dot(tmp);  // i.e. |w-v|^2 -  avoid a sqrt
            if(l2 == 0.0f)
            {
                return (p - v).norm();   // v == w case
            }

            // v + t (w - v)
            // Project point p onto the line:
            // t = [(p-v) . (w-v)] / |w-v|^2
            const float t = ((p - v).dot(w - v)) / l2;
            if(t < 0.0)
            {
                return (p, v).norm();  // Beyond v end of the segment
            }
            else if(t > 1.0)
            {
                return (p, w).norm();  // Beyond w end of the segment
            }

            // Projection falls on the segment
            const Eigen::Vector2f projection = v + t * (w - v);
            return (p - projection).norm();
        }
    }
}

#endif

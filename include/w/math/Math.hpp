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
#include <vector>

namespace w
{
    const float Pi = M_PI;

    namespace math
    {
        // Ment to be used only within this namespace
        namespace private_
        {
            static bool lineToLineIntersection(const Eigen::Vector2f & vertex1, const Eigen::Vector2f & vertex2,
                const Eigen::Vector2f & vertex3, const Eigen::Vector2f & vertex4, float & r, float & s);
        }

        static Eigen::Matrix4f lookAt(const Eigen::Vector3f& position2, const Eigen::Vector3f& target, const Eigen::Vector3f& up)
        {
            /*
             *
             * TODO
             *
            Eigen::Vector3f cameraPosition = cameraPositionOrig;
            Eigen::Vector3f zaxis = (cameraTarget - cameraPosition).normalized();
            Eigen::Vector3f xaxis = (cameraUpVector.cross(zaxis)).normalized();
            Eigen::Vector3f yaxis = zaxis.cross(xaxis);

            Eigen::Matrix4f m;
            m(0,0) = xaxis.x();
            m(1,0) = xaxis.y();
            m(2,0) = xaxis.z();
            m(3,0) = -xaxis.dot(cameraPosition);

            m(0,0) = yaxis.x();
            m(1,0) = yaxis.y();
            m(2,0) = yaxis.z();
            m(3,0) = -yaxis.dot(cameraPosition);

            m(0,0) = zaxis.x();
            m(1,0) = zaxis.y();
            m(2,0) = zaxis.z();
            m(3,0) = -zaxis.dot(cameraPosition);

            m(0,0) = 0.0f;
            m(1,0) = 0.0f;
            m(2,0) = 0.0f;
            m(3,0) = 1.0f;

            return m;*/
            /*
            Eigen::Vector3f position = -position2;

            Eigen::Matrix3f tmp;
            tmp.col(2) = (position-target).normalized();
            tmp.col(0) = up.cross(tmp.col(2)).normalized();
            tmp.col(1) = tmp.col(2).cross(tmp.col(0));

            Eigen::Matrix4f r;
            r.topLeftCorner<3, 3>() = tmp.transpose();
            r.topRightCorner<3, 1>() = -tmp.transpose() * position;
            return r;*/
            Eigen::Matrix4f r;
            return r;
        }

        static Eigen::Matrix4f view(const Eigen::Vector3f& location, const Eigen::Vector3f& forward, const Eigen::Vector3f& up)
        {
            return Eigen::Affine3f(Eigen::Translation3f(-location)).matrix();
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
         * param [in]   point       point
         * param [in]   p1          line segments start
         * param [in]   p0          line segments end
         * param [out]  closest     closest point on segment for the point
         * returns      distance from point to the closest point
         *
         * Returns minimum distance between line segment vw and point p.
         * returns the closest point on the line segment.
         */
        static float pointDistanceToLine(const Eigen::Vector2f & point, const Eigen::Vector2f & p1, const Eigen::Vector2f & p2, Eigen::Vector2f & closest)
        {
            float dx = p2.x() - p1.x();
            float dy = p2.y() - p1.y();
            if((dx == 0) && (dy == 0))
            {
                // Line is a point!
                closest = p1;
                dx = point.x() - p1.x();
                dy = point.y() - p1.y();
            }
            else
            {
                // Calculate the t that minimizes the distance
                float t = ((point.x() - p1.x()) * dx + (point.y() - p1.y()) * dy) / (dx * dx + dy * dy);

                // See if this represents one of the segment's  end points or
                // a point in the middle.
                if(t < 0)
                {
                    closest = Eigen::Vector2f(p1.x(), p1.y());
                    dx = point.x() - p1.x();
                    dy = point.y() - p1.y();
                }
                else if(t > 1)
                {
                    closest = Eigen::Vector2f(p2.x(), p2.y());
                    dx = point.x() - p2.x();
                    dy = point.y() - p2.y();
                }
                else
                {
                    closest = Eigen::Vector2f(p1.x() + t * dx, p1.y() + t * dy);
                    dx = point.x() - closest.x();
                    dy = point.y() - closest.y();
                }
            }

            return sqrt(dx * dx + dy * dy);
        }

        /**
         * param [in]   rayOrigin           ray origin
         * param [in]   rayDirection        ray direction
         * param [in]   segmentPoint0       line segment point 0
         * param [in]   segmentPoint1       line segment point 1
         *
         * returns      Intersection2D having the intersection points
         */
        static std::vector<Eigen::Vector2f> rayToLineSegmentIntersection(const Eigen::Vector2f & rayOrigin, const Eigen::Vector2f & rayDirection,
            const Eigen::Vector2f & segmentPoint0, const Eigen::Vector2f & segmentPoint1)
        {
            std::vector<Eigen::Vector2f> result;
            float r;
            float s;
            if(private_::lineToLineIntersection(rayOrigin, rayOrigin + rayDirection, segmentPoint0, segmentPoint1, r, s))
            {
                if(r >= 0)
                {
                    if(s >= 0 && s <= 1)
                    {
                        result.push_back(rayOrigin + rayDirection * r);
                    }
                }
            }
            return result;
        }

        static inline float distance(const Eigen::Vector2f & v0, const Eigen::Vector2f & v1)
        {
            return sqrt(
                (v0.x() - v1.x()) * (v0.x() - v1.x()) +
                (v0.y() - v1.y()) * (v0.y() - v1.y()));
        }

        static inline float distance(const Eigen::Vector3f & v0, const Eigen::Vector3f & v1)
        {
            Eigen::Vector3f tmp = v0 - v1;
            return sqrt(tmp.x() * tmp.x() + tmp.y() * tmp.y() + tmp.z() * tmp.z());
        }
        /**
         * Checks if point is inside rectangle. Rectangle coordinate parameters are
         * ordered in this check!
         *
         * param [in]   p        point
         * param [in]   r0       lower left corner of rectange
         * param [in]   r1       upper right corner of rectange
         *
         * returns      true if point was inside or at the edge of rectangle,
         *              false otherwise
         */
        static inline bool pointInsideOrderedRectange(const Eigen::Vector2f & p, const Eigen::Vector2f & r0, const Eigen::Vector2f & r1)
        {
            if(r0.x() <= p.x() && p.x() <= r1.x() &&
                r0.y() <= p.y() && p.y() <= r1.y())
            {
                return true;
            }
            return false;
        }

        // Ment to be used only within this namespace
        namespace private_
        {
            static bool lineToLineIntersection(const Eigen::Vector2f & vertex1, const Eigen::Vector2f & vertex2,
                const Eigen::Vector2f & vertex3, const Eigen::Vector2f & vertex4, float & r, float & s)
            {
                float d;

                // Check that lines aren't parallel
                Eigen::Vector2f vertex1to2 = vertex2 - vertex1;
                Eigen::Vector2f vertex3to4 = vertex4 - vertex3;

                if(vertex1to2.y() / vertex1to2.x() != vertex3to4.y() / vertex3to4.x())
                {
                    d = vertex1to2.x() * vertex3to4.y() - vertex1to2.y() * vertex3to4.x();
                    if(d != 0)
                    {
                        Eigen::Vector2f vertex3to1 = vertex1 - vertex3;
                        r = (vertex3to1.y() * vertex3to4.x() - vertex3to1.x() * vertex3to4.y()) / d;
                        s = (vertex3to1.y() * vertex1to2.x() - vertex3to1.x() * vertex1to2.y()) / d;
                        return true;
                    }
                }
                return false;
            }
        }
    }
}

#endif

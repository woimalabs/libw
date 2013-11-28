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
            //Eigen::Vector3f transformedForward = forward; //rotation.transformVector(forward, forward);
            //Eigen::Vector3f cameraLookAt = location + transformedForward;
            //return lookAt(location, cameraLookAt, Eigen::Vector3f(0.0f, 1.0f, 0.0f));
            return Eigen::Matrix4f::Identity();
        }

        static Eigen::Matrix4f perspective(float fovY, float aspect, float near, float far)
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

        static Eigen::Matrix4f ortho(float left, float right, float bottom, float top, float near, float far)
        {
            float width(right - left);
            float height(top - bottom);
            float depth(far - near);

            Eigen::Matrix4f o = Eigen::Matrix4f::Zero();
            o(0, 0) = 2.0f / width;
            o(0, 3) = (right + left) / width;
            o(1, 1) = 2.0f / height;
            o(1, 3) = (top + bottom) / height;
            o(2, 2) = -2.0f / depth;
            o(2, 3) = (far + near) / depth;

            return o;
        }

        /* (void)loadOrthoMatrix:(GLfloat *)matrix left:(GLfloat)left right:(GLfloat)right bottom:(GLfloat)bottom top:(GLfloat)top near:(GLfloat)near far:(GLfloat)far;
        {
            GLfloat r_l = right - left;
            GLfloat t_b = top - bottom;
            GLfloat f_n = far - near;
            GLfloat tx = - (right + left) / (right - left);
            GLfloat ty = - (top + bottom) / (top - bottom);
            GLfloat tz = - (far + near) / (far - near);

            matrix[0] = 2.0f / r_l;
            matrix[1] = 0.0f;
            matrix[2] = 0.0f;
            matrix[3] = tx;

            matrix[4] = 0.0f;
            matrix[5] = 2.0f / t_b;
            matrix[6] = 0.0f;
            matrix[7] = ty;

            matrix[8] = 0.0f;
            matrix[9] = 0.0f;
            matrix[10] = 2.0f / f_n;
            matrix[11] = tz;

            matrix[12] = 0.0f;
            matrix[13] = 0.0f;
            matrix[14] = 0.0f;
            matrix[15] = 1.0f;
        }*/

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

        static Eigen::Matrix4f orthogonal(float left, float right, float bottom, float top, float near, float far)
        {
            Eigen::Matrix4f r = Eigen::Matrix4f::Identity();

            float rl = right - left;
            float tb = top - bottom;
            float fn = far - near;
            float tx = -(right + left) / (right - left);
            float ty = -(top + bottom) / (top - bottom);
            float tz = -(far + near) / (far - near);

            r(0, 0) = 2.0f / rl;
            r(0, 3) = tx;

            r(1, 1) = 2.0f / tb;
            r(1, 3) = ty;

            r(2, 2) = 2.0f / fn;
            r(2, 3) = tz;

            r(3, 3) = 1.0f;

            return r;
        }
    }
}

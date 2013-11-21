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

#ifndef LIBW_MATH_VECTOR4
#define LIBW_MATH_VECTOR4

namespace w
{
    class Vector4depr
    {
    public:
        Vector4depr(float x, float y, float z, float w):
            x_(x),
            y_(y),
            z_(z),
            w_(w)
        {
        }

        Vector4depr(Vector4depr const& r):
            x_(r.x_),
            y_(r.y_),
            z_(r.z_),
            w_(r.w_)
        {
        }

        ~Vector4depr()
        {
        }

        Vector4depr& operator=(Vector4depr const& r)
        {
            if (this != &r)
            {
                x_ = r.x_;
                y_ = r.y_;
                z_ = r.z_;
                w_ = r.w_;
            }
            return *this;
        }

        float x()
        {
            return x_;
        }

        float y()
        {
            return y_;
        }

        float z()
        {
            return z_;
        }

        float w()
        {
            return w_;
        }

    private:
        float x_;
        float y_;
        float z_;
        float w_;
    };
}

#endif

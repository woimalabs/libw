/**
 * libw
 *
 * Copyright (C) 2013-2015 Woima Solutions
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

#ifndef LIBW_GRAPHICS_BITMAPPRIVATE
#define LIBW_GRAPHICS_BITMAPPRIVATE

#include "w/graphics/Bitmap.hpp"
#include "w/base/Class.hpp"
#include "w/base/Referenced.hpp"
#include "w/math/Eigen.hpp"

namespace w
{
    namespace graphics
    {
        class BitmapPrivate: public Referenced
        {
        public:
            UNCOPYABLE(BitmapPrivate)

            BitmapPrivate(unsigned int width, unsigned int height, Bitmap::Format::Enum format);
            ~BitmapPrivate();
            char* data() const;
            unsigned int width() const;
            unsigned int height() const;
            Bitmap::Format::Enum format() const;

        private:
            unsigned int width_;
            unsigned int height_;
            Bitmap::Format::Enum format_;
            char* data_;
        };
    }
}

#endif

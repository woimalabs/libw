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

#include "BitmapPrivate.hpp"
#include "w/graphics/FrameBuffer.hpp"
#include <w/base/Exception.hpp>

namespace w
{
    namespace graphics
    {
        BitmapPrivate::BitmapPrivate(unsigned int width, unsigned int height, Bitmap::Format::Enum format):
            width_(width),
            height_(height),
            format_(format),
            data_(NULL)
        {
            unsigned int bytesPerPixel = 0;
            switch(format_)
            {
                case Bitmap::Format::LUMINANCE_8:
                {
                    bytesPerPixel = 1;
                    break;
                }
                case Bitmap::Format::RGB_888:
                {
                    bytesPerPixel = 3;
                    break;
                }
                case Bitmap::Format::RGBA_8888:
                {
                    bytesPerPixel = 4;
                    break;
                }
            }
            data_ = new char[width_ * height * bytesPerPixel];
        }

        BitmapPrivate::~BitmapPrivate()
        {
            delete [] data_;
        }

        char* BitmapPrivate::data()
        {
            return data_;
        }
    }
}

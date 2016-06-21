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

#ifndef LIBW_GRAPHICS_BITMAP
#define LIBW_GRAPHICS_BITMAP

#include "w/base/Class.hpp"
#include "w/base/ReferencedPointer.hpp"
#include "w/math/Eigen.hpp"
#include <string>

namespace w
{
    namespace graphics
    {
        class Bitmap: public ReferencedPointer<class BitmapPrivate>
        {
        public:
            COPYABLE(Bitmap)

            struct Format
            {
                enum Enum
                {
                    Undefined,
                    LUMINANCE_8,
                    RGB_888,
                    RGBA_8888
                };
            };

            /**
             * Bitmap object to hold raw data
             */
            Bitmap(unsigned int width, unsigned int height, Format::Enum format);
            
            /**
             * Bitmap object from bundled file
             */
            Bitmap(const std::string & file);
            ~Bitmap();
            unsigned int width() const;
            unsigned int height() const;
            Format::Enum format() const;
            char* data() const;
        };
    }
}

#endif

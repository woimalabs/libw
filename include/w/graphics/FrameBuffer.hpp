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

#ifndef LIBW_GRAPHICS_FRAMEBUFFER
#define LIBW_GRAPHICS_FRAMEBUFFER

#include "w/base/Class.hpp"
#include "w/base/ReferencedPointer.hpp"
#include "w/math/Eigen.hpp"

namespace w
{
    namespace graphics
    {
        class FrameBuffer: public ReferencedPointer<class FrameBufferPrivate>
        {
        public:
            COPYABLE(FrameBuffer)

            /**
             * Frame buffer is not supporting depth because it's
             * unsupported in many Android GLES2 drivers.
             */
            struct Format
            {
                enum Enum
                {
                    LUMINANCE_8,
                    RGB_888,
                    RGBA_8888
                };
            };

            /**
             * Creates a frame buffer object for offscreen rendering
             *
             * @note This class needs to be constructed in thread with GL context.
             */
            FrameBuffer(unsigned int width, unsigned int height, Format::Enum format, const Eigen::Vector4f& clearColor);
            ~FrameBuffer();
            void clearBuffer();
            void bind();
        };
    }
}

#endif

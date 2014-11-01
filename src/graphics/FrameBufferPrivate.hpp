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

#ifndef LIBW_GRAPHICS_FRAMEBUFFERPRIVATE
#define LIBW_GRAPHICS_FRAMEBUFFERPRIVATE

#include "w/graphics/FrameBuffer.hpp"
#include "w/base/Class.hpp"
#include "w/base/Referenced.hpp"
#include "w/math/Eigen.hpp"
#ifdef linux
    #include <GLES2/gl2.h>
    #include <GLES2/gl2ext.h>
#endif

namespace w
{
    namespace graphics
    {
        class FrameBufferPrivate: public Referenced
        {
        public:
            UNCOPYABLE(FrameBufferPrivate)
            EIGEN_MAKE_ALIGNED_OPERATOR_NEW

            FrameBufferPrivate(unsigned int width, unsigned int height, FrameBuffer::Format::Enum format, const Eigen::Vector4f& clearColor);
            ~FrameBufferPrivate();
            unsigned int width() const;
            unsigned int height() const;
            void clearBuffer();
            void bind();

        private:
            std::string name_;
            unsigned int width_;
            unsigned int height_;
            unsigned int format_;
            Eigen::Vector4f clearColor_;
            GLuint frameBufferId_;
            GLuint textureColorId_;
        };
    }
}

#endif

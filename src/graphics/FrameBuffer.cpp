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

#include "w/graphics/FrameBuffer.hpp"
#include "FrameBufferPrivate.hpp"

namespace w
{
    namespace graphics
    {
        FrameBuffer::FrameBuffer(unsigned int width, unsigned int height, Format::Enum format, const Eigen::Vector4f& clearColor):
            ReferencedPointer<FrameBufferPrivate>(new FrameBufferPrivate(width, height, format, clearColor))
        {
        }

        FrameBuffer::FrameBuffer(FrameBuffer const& r):
            ReferencedPointer<FrameBufferPrivate>(r.pointer())
        {
        }

        FrameBuffer::~FrameBuffer()
        {
        }

        FrameBuffer& FrameBuffer::operator=(FrameBuffer const& r)
        {
            ReferencedPointer::operator=(r);
            return *this;
        }

        unsigned int FrameBuffer::width() const
        {
            return pointer()->width();
        }

        unsigned int FrameBuffer::height() const
        {
            return pointer()->height();
        }

        void FrameBuffer::clearBuffer()
        {
            pointer()->clearBuffer();
        }

        void FrameBuffer::bind()
        {
            pointer()->bind();
        }
    }
}

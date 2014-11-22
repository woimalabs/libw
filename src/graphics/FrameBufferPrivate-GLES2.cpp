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

#include "FrameBufferPrivate.hpp"
#include "w/graphics/FrameBuffer.hpp"
#include <w/base/Exception.hpp>

namespace w
{
    namespace graphics
    {
        FrameBufferPrivate::FrameBufferPrivate(unsigned int width, unsigned int height, FrameBuffer::Format::Enum format, const Eigen::Vector4f& clearColor):
            width_(width),
            height_(height),
            format_(format),
            clearColor_(clearColor),
            frameBufferId_(0),
            textureColorId_(0)
        {
        }

        FrameBufferPrivate::~FrameBufferPrivate()
        {
            glDeleteFramebuffers(1, &frameBufferId_);
            glDeleteRenderbuffers(1, &textureColorId_);
        }

        unsigned int FrameBufferPrivate::width() const
        {
            return width_;
        }

        unsigned int FrameBufferPrivate::height() const
        {
            return height_;
        }

        void FrameBufferPrivate::clearBuffer()
        {
            glClearColor(clearColor_.x(), clearColor_.y(), clearColor_.z(), clearColor_.w());
            glClear(GL_COLOR_BUFFER_BIT);
        }

        void FrameBufferPrivate::bind()
        {
            if(frameBufferId_ == 0)
            {
                // Create frame buffer
                glGenFramebuffers(1, &frameBufferId_);

                // Color buffer
                glGenTextures(1, &textureColorId_);
                glBindTexture(GL_TEXTURE_2D, textureColorId_);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorId_, 0);

                // FBO status
                GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
                if(status != GL_FRAMEBUFFER_COMPLETE)
                {
                    throw Exception("FrameBuffer, creation failed!");
                }
            }
            // Render to our framebuffer
            glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId_);

            // Render from lower left to upper right
            glViewport(0, 0, width_, height_);
        }
    }
}

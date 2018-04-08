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

#ifdef __APPLE__
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#include <GLKit/GLKit.h>
#endif

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
            glBindTexture(GL_TEXTURE_2D, textureColorId_);
            glEnable(GL_TEXTURE_2D);
            glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId_);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorId_, 0);
            glViewport(0, 0, width_, height_);
            glClearColor(clearColor_.x(), clearColor_.y(), clearColor_.z(), clearColor_.w());
            glClear(GL_COLOR_BUFFER_BIT);
        }

        void FrameBufferPrivate::bind()
        {
            if(frameBufferId_ == 0)
            {
#ifdef __linux__
                // Create frame buffer
                glGenFramebuffers(1, &frameBufferId_);

                // Color buffer
                glGenTextures(1, &textureColorId_);
                glBindTexture(GL_TEXTURE_2D, textureColorId_);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorId_, 0);

                // FBO status
                GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
                if(status != GL_FRAMEBUFFER_COMPLETE)
                {
                    throw Exception("FrameBuffer, creation failed!");
                }
#elif __APPLE__
                /* reference:
                 https://developer.apple.com/library/ios/documentation/3DDrawing/Conceptual/OpenGLES_ProgrammingGuide/WorkingwithEAGLContexts/WorkingwithEAGLContexts.html#//apple_ref/doc/uid/TP40008793-CH103-SW1
                 */
                
                glGenFramebuffers(1, &frameBufferId_);
                glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId_);
                
                GLuint colorRenderbuffer;
                glGenRenderbuffers(1, &colorRenderbuffer);
                glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
                glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA /* T */, width_, height_);
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer);
                
                GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER) ;
                if(status != GL_FRAMEBUFFER_COMPLETE) {
                    NSLog(@"failed to make complete framebuffer object %x", status);
                }
                
                // create the texture
                glGenTextures(1, &textureColorId_);
                glBindTexture(GL_TEXTURE_2D, textureColorId_);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA /* T */,  width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorId_, 0);
#endif
            }

            // Render to our framebuffer
            glBindTexture(GL_TEXTURE_2D, textureColorId_);
            glEnable(GL_TEXTURE_2D);
            glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId_);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorId_, 0);
            glViewport(0, 0, width_, height_);
        }

        void FrameBufferPrivate::bindAsTexture()
        {
            glBindTexture(GL_TEXTURE_2D, textureColorId_);
        }
    }
}

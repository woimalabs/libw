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

#include "w/graphics/Window.hpp"

#include "WindowPrivate.hpp"
#include <OpenGLES/ES2/gl.h>
#include <GLKit/GLKit.h>

namespace w
{
    namespace graphics
    {
        WindowPrivate::WindowPrivate(const std::string& name, unsigned int width, unsigned int height, const Eigen::Vector4f& clearColor):
            name_(name),
            width_(width),
            height_(height),
            clearColor_(clearColor)
        {
        }

        WindowPrivate::~WindowPrivate()
        {
        }

        void WindowPrivate::clearBuffer()
        {
            glClearColor(clearColor_.x(), clearColor_.y(), clearColor_.z(), clearColor_.w());
            glClearDepthf(1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        void WindowPrivate::resize(unsigned int width, unsigned int height)
        {
        }

        void WindowPrivate::swapBuffers()
        {
        }
        
        void WindowPrivate::bind()
        {
            [((GLKView *) w::graphics::Window::topUIView) bindDrawable];
        }
        
        Bitmap WindowPrivate::screenshot()
        {
            Bitmap r(width_, height_, Bitmap::Format::RGBA_8888);
            glReadPixels(0, 0, width_, height_, GL_RGBA, GL_UNSIGNED_BYTE, r.data());
            return r;
        }
    }
}

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
#include "w/base/Log.hpp"

namespace w
{
    Window::Window(const std::string& name, unsigned int width, unsigned int height, const Eigen::Vector4f& clearColor):
        private_(new WindowPrivate(name, width, height, clearColor))
    {
        LOGD("window!: %d, %d", private_->width(), private_->height());
        private_->increment();
    }

    Window::~Window()
    {
        private_->decrement();
        private_ = NULL;
    }

    Window::Window(Window const &r):
        private_(r.private_)
    {
        private_->increment();
    }

    Window &Window::operator=(Window const &r)
    {
        if (this != &r)
        {
            private_ = r.private_;
            private_->increment();
        }
        return *this;
    }

    unsigned int Window::width() const
    {
        return private_->width();
    }

    unsigned int Window::height() const
    {
        return private_->height();
    }

    void Window::setClearColor(const Eigen::Vector4f& clearColor)
    {
        private_->setClearColor(clearColor);
    }

    void Window::clearBuffer()
    {
        private_->clearBuffer();
    }

    void Window::swapBuffers()
    {
        private_->swapBuffers();
    }

    void Window::resize(unsigned int width, unsigned int height)
    {
        private_->resize(width, height);
    }

    #if defined(linux) && !defined(__ANDROID__)
        Display* Window::xDisplay() const
        {
            return private_->xDisplay();
        }
    #endif
}

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

#include "w/Class.hpp"
#include "w/math/Vector4.hpp"
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <string>

namespace w
{
    class WindowPrivate
    {
    public:
        UNCOPYABLE(WindowPrivate)

        WindowPrivate(const std::string& name, unsigned int width, unsigned int height, const Vector4& clearColor);
        ~WindowPrivate();
        void clearBuffer();
        void swapBuffers();
        void resize(unsigned int width, unsigned int height);
#ifdef ANDROID

#elif linux
        Display* xDisplay() const;
#endif

    private:
        std::string name_;
        unsigned int width_;
        unsigned int height_;
        Vector4 clearColor_;
#ifdef ANDROID

#elif linux
        bool supportsGL2();
        bool makeCurrentContext();

        // EGL
        bool EGLDisplayCheck();
        bool EGLConfigCheck();
        bool EGLSurfaceCheck();
        EGLDisplay EGLDisplay_;
        EGLSurface EGLSurface_;
        EGLConfig EGLConfig_;
        EGLContext EGLContext_;

        // X
        bool ensureXWindow();
        XVisualInfo *xVisualInfo();
        Window xWindow_;
        Display* xDisplay_;
#endif
    };
}

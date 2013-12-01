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

#include "w/base/Class.hpp"
#include "w/base/Referenced.hpp"
#include "w/base/Log.hpp"
#include "w/math/Eigen.hpp"
#ifdef linux
    #include <EGL/egl.h>
    #include <GLES2/gl2.h>
    #include <GLES2/gl2ext.h>
#endif
#include <string>

namespace w
{
    class WindowPrivate: public Referenced
    {
    public:
        UNCOPYABLE(WindowPrivate)
        EIGEN_MAKE_ALIGNED_OPERATOR_NEW

        WindowPrivate(const std::string& name, unsigned int width, unsigned int height, const Eigen::Vector4f& clearColor);
        ~WindowPrivate();

        unsigned int width() const
        {
            LOGD("width: %d", width_);
            return width_;
        }

        unsigned int height() const
        {
            LOGD("height: %d", height_);
            return height_;
        }

        void clearBuffer();
        void swapBuffers();
        void resize(unsigned int width, unsigned int height);
        #ifdef ANDROID
            //
        #elif linux
            Display* xDisplay() const;
        #endif

    private:
        std::string name_;
        unsigned int width_;
        unsigned int height_;
        Eigen::Vector4f clearColor_;
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

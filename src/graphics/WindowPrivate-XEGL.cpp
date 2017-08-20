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

#include "WindowPrivate.hpp"
#include "w/base/String.hpp"
#include <w/base/Exception.hpp>
#include <w/base/Log.hpp>
#include <w/math/Eigen.hpp>
#include <fstream>
#include <sstream>

namespace w
{
    namespace graphics
    {
        WindowPrivate::WindowPrivate(const std::string& name, unsigned int width, unsigned int height, const Eigen::Vector4f& clearColor):
            name_(name),
            width_(width),
            height_(height),
            clearColor_(clearColor),
            EGLDisplay_(EGL_NO_DISPLAY),
            EGLSurface_(EGL_NO_SURFACE),
            EGLConfig_(0),
            EGLContext_(EGL_NO_CONTEXT),
            xWindow_(0),
            xDisplay_(0)
        {
            xDisplay_ = XOpenDisplay(NULL);
            if (!xDisplay_)
            {
                throw Exception("Could not open XDisplay.");
            }
            resize(width_, height_);

            if (!xWindow_)
            {
                throw Exception("No X Window.");
            }

            if (!makeCurrentContext())
            {
                throw Exception("Could not make context current.");
            }

            if (!supportsGL2())
            {
                throw Exception("OpenGL(ES) version >= 2.0 needed.");
            }
        }

        WindowPrivate::~WindowPrivate()
        {
            eglMakeCurrent(EGLDisplay_, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
            eglDestroyContext(EGLDisplay_, EGLContext_);
            eglDestroySurface(EGLDisplay_, EGLContext_);
            eglTerminate(EGLDisplay_);
        }

        void WindowPrivate::clearBuffer()
        {
            glClearColor(clearColor_.x(), clearColor_.y(), clearColor_.z(), clearColor_.w());
            glClearDepthf(1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        void WindowPrivate::resize(unsigned int width, unsigned int height)
        {
            // Window and size change
            if (xWindow_)
            {
                if (width_ != width || height_ != height)
                {
                    XDestroyWindow(xDisplay_, xWindow_);
                    xWindow_ = 0;
                }
                else
                {
                    return;
                }
            }

            // Create window
            width_ = width;
            height_ = height;
            if (!createXWindow())
            {
                throw Exception("Couldn't create X Window!");
            }

            XMapWindow(xDisplay_, xWindow_);

            // Update GL
            glViewport(0, 0, width_, height_);
        }

        void WindowPrivate::bind()
        {
            // Render to our framebuffer
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            // Render from lower left to upper right
            glViewport(0, 0, width_, height_);
        }

        void WindowPrivate::swapBuffers()
        {
            eglSwapBuffers(EGLDisplay_, EGLSurface_);
        }

        Display* WindowPrivate::xDisplay() const
        {
            return xDisplay_;
        }

        bool WindowPrivate::supportsGL2()
        {
            bool r = false;

            const GLubyte* tmpVersion = glGetString(GL_VERSION);
            if (tmpVersion == NULL)
            {
                LOGE("WindowPrivate supportsGL2 glGetString == NULL (?)");
                return false;
            }
            std::string tmp((const char*)tmpVersion);

            int glMajor(0);
            size_t pointPosition(tmp.find('.'));

            if (pointPosition != std::string::npos)
            {
                pointPosition--;
                size_t startPosition(tmp.rfind(' ', pointPosition));
                if (startPosition == std::string::npos)
                {
                    startPosition = 0;
                }
                else
                {
                    startPosition++;
                }
                glMajor = String::toInt(tmp.substr(startPosition, pointPosition - startPosition + 1));
            }
            r = glMajor >= 2;

            return r;
        }

        bool WindowPrivate::makeCurrentContext()
        {
            if (!EGLSurfaceCheck())
            {
                LOGE("EGLSurfaceCheck failed");
                return false;
            }

            if (EGLContext_ == eglGetCurrentContext())
            {
                return true;
            }

            if (!eglMakeCurrent(EGLDisplay_, EGLSurface_, EGLSurface_, EGLContext_))
            {
                LOGE("eglMakeCurrent failed, error %d", eglGetError());
                return false;
            }

            if (!eglSwapInterval(EGLDisplay_, 0))
            {
                LOGE("eglSwapInterval failed! Continuing...");
            }

            return true;
        }

        bool WindowPrivate::EGLDisplayCheck()
        {
            if (EGLDisplay_)
            {
                return true;
            }

            EGLDisplay_ = eglGetDisplay((EGLNativeDisplayType) xDisplay_);
            if (!EGLDisplay_)
            {
                LOGE("eglGetDisplay() failed: %d", eglGetError());
                return false;
            }

            if (!eglInitialize(EGLDisplay_, NULL, NULL))
            {
                LOGE("eglInitialize() failed: %d", eglGetError());
                EGLDisplay_ = 0;
                return false;
            }

            return true;
        }

        bool WindowPrivate::EGLConfigCheck()
        {
            static const EGLint attributes[] = {
                EGL_RED_SIZE, 1,
                EGL_GREEN_SIZE, 1,
                EGL_BLUE_SIZE, 1,
                EGL_ALPHA_SIZE, 1,
                EGL_DEPTH_SIZE, 1,
                EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
                EGL_NONE
            };
            EGLint num_configs;
            EGLint vid;

            if (EGLConfig_)
            {
                return true;
            }

            if (!EGLDisplayCheck())
            {
                return false;
            }

            if (!eglChooseConfig(EGLDisplay_, attributes, &EGLConfig_, 1, &num_configs))
            {
                LOGE("eglChooseConfig() failed with error: %d", eglGetError());
                return false;
            }

            if (!eglGetConfigAttrib(EGLDisplay_, EGLConfig_, EGL_NATIVE_VISUAL_ID, &vid))
            {
                LOGE("eglGetConfigAttrib() failed with error: %d", eglGetError());
                return false;
            }

            // Debug
            {
                int red;
                int green;
                int blue;
                int alpha;
                int depth;
                int buf;
                int configId;
                int nativeId;
                int sampleBuffers;
                int samples;
                eglGetConfigAttrib(EGLDisplay_, EGLConfig_, EGL_RED_SIZE, &red);
                eglGetConfigAttrib(EGLDisplay_, EGLConfig_, EGL_GREEN_SIZE, &green);
                eglGetConfigAttrib(EGLDisplay_, EGLConfig_, EGL_BLUE_SIZE, &blue);
                eglGetConfigAttrib(EGLDisplay_, EGLConfig_, EGL_ALPHA_SIZE, &alpha);
                eglGetConfigAttrib(EGLDisplay_, EGLConfig_, EGL_DEPTH_SIZE, &depth);
                eglGetConfigAttrib(EGLDisplay_, EGLConfig_, EGL_BUFFER_SIZE, &buf);
                eglGetConfigAttrib(EGLDisplay_, EGLConfig_, EGL_CONFIG_ID, &configId);
                eglGetConfigAttrib(EGLDisplay_, EGLConfig_, EGL_NATIVE_VISUAL_ID, &nativeId);
                eglGetConfigAttrib(EGLDisplay_, EGLConfig_, EGL_SAMPLE_BUFFERS, &sampleBuffers);
                eglGetConfigAttrib(EGLDisplay_, EGLConfig_, EGL_SAMPLES, &samples);
                /*
                LOGI("EGL Config Attibutes:\n"
                    " EGL_RED_SIZE: %d\n"
                    " EGL_GREEN_SIZE: %d\n"
                    " EGL_BLUE_SIZE: %d\n"
                    " EGL_ALPHA_SIZE: %d\n"
                    " EGL_DEPTH_SIZE: %d\n"
                    " EGL_BUFFER_SIZE: %d\n"
                    " EGL_CONFIG_ID: %d\n"
                    " EGL_NATIVE_VISUAL_ID: %d\n"
                    " EGL_SAMPLE_BUFFERS: %d\n"
                    " EGL_SAMPLES: %d",
                    red, green, blue, alpha, depth, buf, configId, nativeId, sampleBuffers, samples);
                    */
            }

            return true;
        }

        bool WindowPrivate::EGLSurfaceCheck()
        {
            static const EGLint contextAttributes[] =
            {
                EGL_CONTEXT_CLIENT_VERSION, 2,
                EGL_NONE
            };

            if (EGLSurface_)
            {
                return true;
            }

            if (!EGLDisplayCheck())
            {
                return false;
            }

            eglBindAPI(EGL_OPENGL_ES_API);

            EGLContext_ = eglCreateContext(EGLDisplay_, EGLConfig_, EGL_NO_CONTEXT, contextAttributes);
            if (!EGLContext_)
            {
                LOGE("eglCreateContext() failed with error: %d", eglGetError());
                return false;
            }

            EGLSurface_ = eglCreateWindowSurface(EGLDisplay_, EGLConfig_, (EGLNativeWindowType)xWindow_, NULL);
            if (!EGLSurface_)
            {
                LOGE("eglCreateWindowSurface failed with error: %d", eglGetError());
                return false;
            }

            return true;
        }

        bool WindowPrivate::createXWindow()
        {
            if (xWindow_)
            {
                return true;
            }

            if (!xDisplay_)
            {
                LOGE("X11 Display has not been initialized!");
                return false;
            }

            XVisualInfo* visualInfo = xVisualInfo();
            if (!visualInfo)
            {
                LOGE("Could not get a valid XVisualInfo!");
                return false;
            }

            //LOGI("Creating XWindow W:%d H:%d VisualID:0x%x", width_, height_, visualInfo->visualid);

            // Create X Window
            XSetWindowAttributes attributes;
            unsigned long mask;
            Window root = RootWindow(xDisplay_, DefaultScreen(xDisplay_));
            attributes.background_pixel = 0;
            attributes.border_pixel = 0;
            attributes.colormap = XCreateColormap(xDisplay_, root, visualInfo->visual, AllocNone);
            attributes.event_mask = KeyPressMask | KeyReleaseMask | PointerMotionMask | ButtonPressMask | ButtonReleaseMask;
            mask = CWBackPixel | CWBorderPixel | CWColormap | CWEventMask;
            xWindow_ = XCreateWindow(xDisplay_, root, 0, 0, width_, height_,
                0, visualInfo->depth, InputOutput, visualInfo->visual, mask,
                &attributes);

            XFree(visualInfo);
            if (!xWindow_)
            {
                LOGE("XCreateWindow() failed!");
                return false;
            }

            // Size hints
            XSizeHints sizehints;
            sizehints.min_width = width_;
            sizehints.min_height = height_;
            sizehints.max_width = width_;
            sizehints.max_height = height_;
            sizehints.flags = PMaxSize | PMinSize;
            XSetNormalHints(xDisplay_, xWindow_, &sizehints);
            XSetStandardProperties(xDisplay_, xWindow_, name_.c_str(), name_.c_str(), None, NULL, 0, &sizehints);

            // Window Delete event from window manager
            Atom wmDelete = XInternAtom(xDisplay_, "WM_DELETE_WINDOW", True);
            XSetWMProtocols(xDisplay_, xWindow_, &wmDelete, 1);

            return true;
        }

        XVisualInfo* WindowPrivate::xVisualInfo()
        {
            XVisualInfo infoTemplate;
            XVisualInfo* info;
            int num;
            EGLint vid;

            if (!EGLConfigCheck())
            {
                LOGE("Failed EGLConfigCheck()");
                return 0;
            }

            if (!eglGetConfigAttrib(EGLDisplay_, EGLConfig_, EGL_NATIVE_VISUAL_ID, &vid))
            {
                LOGE("Failed eglGetConfigAttrib(): %d", eglGetError());
                return 0;
            }

            // The X window visual must match the EGL config
            infoTemplate.visualid = vid;
            info = XGetVisualInfo(xDisplay_, VisualIDMask, &infoTemplate, &num);

            if (!info)
            {
                LOGE("Couldn't get X visual");
                return 0;
            }

            return info;
        }

        Bitmap WindowPrivate::screenshot()
        {
            Bitmap r(width_, height_, Bitmap::Format::RGBA_8888);
            glFlush();
            glReadPixels(0, 0, width_, height_, GL_RGBA, GL_UNSIGNED_BYTE, r.data());
            return r;
        }
    }
}

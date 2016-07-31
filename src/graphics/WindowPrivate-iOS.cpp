/**
 * libw
 *
 * Copyright (C) 2013-2016 Woima Solutions
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
#import <Foundation/Foundation.h>
#import <CoreGraphics/CoreGraphics.h>
#import <UIKit/UIKit.h>

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

        CGContextRef newContext(CGImageRef image)
        {
            size_t bitsPerPixel = 32;
            size_t bitsPerComponent = 8;
            size_t bytesPerPixel = bitsPerPixel / bitsPerComponent;
            size_t width = CGImageGetWidth(image);
            size_t height = CGImageGetHeight(image);
            size_t bytesPerRow = width * bytesPerPixel;
            size_t bufferLength = bytesPerRow * height;

            // RGB(A) device
            CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
            if (!colorSpace) {
                throw Exception("Allocating color space failed.");
                return NULL;
            }
            
            // Bitmap
            uint32_t* bitmapData = (uint32_t*)malloc(bufferLength);
            if (!bitmapData) {
                CGColorSpaceRelease(colorSpace);
                throw Exception("Allocating memory for bitmap failed.");
                return NULL;
            }
            
            // Context
            CGContextRef context = CGBitmapContextCreate(bitmapData, width,
                height, bitsPerComponent, bytesPerRow, colorSpace,
                kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big );
            
            if (!context) {
                CGColorSpaceRelease(colorSpace);
                free(bitmapData);
                throw Exception("Bitmap context creation failed.");
            }
            CGContextSetBlendMode(context, kCGBlendModeCopy); // clean allocated mem
            CGAffineTransform flipVertical =
                CGAffineTransformMake(1, 0, 0, -1, 0, height); // rotate 180
            CGContextConcatCTM(context, flipVertical);
            
            CGColorSpaceRelease(colorSpace);
            
            return context;
        }

        Bitmap WindowPrivate::screenshot()
        {
            //Bitmap r(width_, height_, Bitmap::Format::RGBA_8888);
            //glReadPixels(0, 0, width_, height_, GL_RGBA, GL_UNSIGNED_BYTE, r.data());
            //return r;

            // Get UIImage
            GLKView *glkView = (GLKView*)[self view];
            UIImage *snapshot = [glkView snapshot];
            CGImageRef image = snapshot->CGImage;
            size_t w = CGImageGetWidth(image);
            size_t h = CGImageGetHeight(image);
            CGRect rect = CGRectMake(0, 0, w, h);

            // Image to context
            CGContextRef context = newContext(image);
            CGContextDrawImage(context, rect, image);
            CGImageRelease(image);
            
            // Pointer to the data
            unsigned char *imageData = (unsigned char*)CGBitmapContextGetData(context);
            
            // Copy data and release allocated memory
            size_t bytesPerRow = CGBitmapContextGetBytesPerRow(context);
            unsigned char *rgbaData = NULL;
            if (imageData != NULL) {
                rgbaData = imageData;
            } else {
                throw Exception("Bitmap pixel data not got.");
            }
            
            // Release context
            CGContextRelease(context);

            return new Bitmap(rgbaData, w, h, 4, true);
        }
    }
}

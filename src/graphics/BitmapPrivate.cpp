/**
 * libw
 *
 * Copyright (C) 2013-2015 Woima Solutions
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

#include "BitmapPrivate.hpp"
#include "w/graphics/FrameBuffer.hpp"
#include <w/base/Exception.hpp>
#include "w/base/FileHandle.hpp"
#include <w/base/ResourceManager.hpp>
#include <png.h>

namespace w
{
    namespace graphics
    {
        // Callback function for libpng
        static void read(png_structp p, png_bytep data, png_size_t length);

        BitmapPrivate::BitmapPrivate(unsigned int width, unsigned int height, Bitmap::Format::Enum format):
            width_(width),
            height_(height),
            format_(format),
            data_(NULL)
        {
            unsigned int bytesPerPixel = 0;
            switch(format_)
            {
                case Bitmap::Format::LUMINANCE_8:
                {
                    bytesPerPixel = 1;
                    break;
                }
                case Bitmap::Format::RGB_888:
                {
                    bytesPerPixel = 3;
                    break;
                }
                case Bitmap::Format::RGBA_8888:
                {
                    bytesPerPixel = 4;
                    break;
                }
            }
            data_ = new char[width_ * height * bytesPerPixel];
        }
        
        BitmapPrivate::BitmapPrivate(const std::string & file):
            width_(0),
            height_(0),
            format_(Bitmap::Format::Undefined),
            data_(NULL)
        {
            // TODO: Polish, this code is similar to one in TextureAssetPrivate.cpp
            static const int flags = PNG_TRANSFORM_STRIP_16 |
                PNG_TRANSFORM_GRAY_TO_RGB | PNG_TRANSFORM_PACKING |
                PNG_TRANSFORM_EXPAND;
            
            // Create libpng structs
            png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
            if(!png)
            {
                LOGE("Cannot open file %.!", file.c_str());
                throw Exception("Couldn't create libpng read struct.");
            }
            
            png_infop info = png_create_info_struct(png);
            if(!info)
            {
                LOGE("Cannot open file %.!", file.c_str());
                throw Exception("Couldn't create libpng info struct.");
            }
            
            // Errors, TODO: check if this is needed.
            if(setjmp(png_jmpbuf(png)))
            {
                LOGE("libpng error while reading file %s.", file.c_str());
                throw Exception("Could not load png file!");
            }
            
            // Start reading the file
            ReferencedPointer<FileHandle> fileHandle = ResourceManager::bundledFile(file);

            // Read the image header and data
            png_set_read_fn(png, reinterpret_cast<void*>(fileHandle.pointer()), read);
            png_read_png(png, info, flags, 0);
            png_bytepp rows = png_get_rows(png, info);
            
            // Set pixel format
            unsigned int bytesPerPixel = 0;
            if(png_get_color_type(png, info) == PNG_COLOR_TYPE_RGB)
            {
                format_ = Bitmap::Format::RGB_888;
                bytesPerPixel = 3;
            }
            else if(png_get_color_type(png, info) == PNG_COLOR_TYPE_RGBA)
            {
                format_ = Bitmap::Format::RGBA_8888;
                bytesPerPixel = 4;
            }
            else
            {
                LOGE("Cannot open file %.!", file.c_str());
                throw Exception("libpng unsupported color type.");
            }
            
            // Copy to continous memory
            width_ = png_get_image_width(png, info);
            height_ = png_get_image_height(png, info);
            unsigned int dataSize = width_ * height_ * bytesPerPixel;
            data_ = new char[dataSize];
            for(unsigned int i = 0; i < dataSize; i++)
            {
                data_[i] = 0;
            }
            for(unsigned int i = 0; i < height_; i++)
            {
                memcpy(&(data_)[width_ * bytesPerPixel * i], rows[height_ - i - 1], width_ * bytesPerPixel);
            }
            
            // Free libpng's struct
            png_destroy_read_struct(&png, &info, 0);
        }

        BitmapPrivate::~BitmapPrivate()
        {
            delete [] data_;
        }

        char* BitmapPrivate::data() const
        {
            return data_;
        }
        
        unsigned int BitmapPrivate::width() const
        {
            return width_;
        }
        
        unsigned int BitmapPrivate::height() const
        {
            return height_;
        }
        
        Bitmap::Format::Enum BitmapPrivate::format() const
        {
            return format_;
        }
        
        void read(png_structp fileHandlePointer, png_bytep data, png_size_t length)
        {
            FileHandle* fileHandle = reinterpret_cast<FileHandle*>(png_get_io_ptr(fileHandlePointer));
            fileHandle->read(reinterpret_cast<char*>(data), length);
        }
    }
}

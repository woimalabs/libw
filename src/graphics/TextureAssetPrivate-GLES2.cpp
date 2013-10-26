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

#include "TextureAssetPrivate.hpp"
#include "UniquePointer.hpp"
#include <w/Log.hpp>
#include <w/Exception.hpp>
#include "ResourceManagerPrivate.hpp"
#include <memory>

namespace w
{
    static void read(png_structp p, png_bytep data, png_size_t length);

    TextureAssetPrivate::TextureAssetPrivate(const std::string& file):
        Resource(file),
        bytesPerPixel_(0),
        width_(0),
        height_(0)
    {
        load();
    }

    TextureAssetPrivate::~TextureAssetPrivate()
    {
    }

    void TextureAssetPrivate::load()
    {
        static const int flags = PNG_TRANSFORM_STRIP_16 |
        PNG_TRANSFORM_GRAY_TO_RGB | PNG_TRANSFORM_PACKING |
        PNG_TRANSFORM_EXPAND;

        // Create libpng structs
        png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
        if (!png)
        {
            LOGE("Cannot open file %.!", filename().c_str());
            throw Exception("Couldn't create libpng read struct.");
        }

        png_infop info = png_create_info_struct(png);
        if (!info)
        {
            LOGE("Cannot open file %.!", filename().c_str());
            throw Exception("Couldn't create libpng info struct.");
        }

        // Errors, TODO: check is this needed!
        if (setjmp(png_jmpbuf(png)))
        {
            LOGE("libpng error while reading file %s.", filename().c_str());
            throw Exception("Could not load png file!");
        }

        // Start reading the file
        UniquePointer<FileHandle> fileHandle(ResourceManagerPrivate::getFileHandle(filename()));

        // Read the image header and data
        png_set_read_fn(png, reinterpret_cast<void*>(fileHandle.pointer()), read);
        png_read_png(png, info, flags, 0);
        png_bytepp rows = png_get_rows(png, info);

        // Copy to continous memory
        width_ = png_get_image_width(png, info);
        height_ = png_get_image_height(png, info);
        char* tmpData = new char[width_ * height_ * bytesPerPixel_];
        for (unsigned int i = 0; i < height_; i++)
        {
            memcpy(&tmpData[bytesPerPixel_ * width_ * i], rows[height_ - i - 1], width_ * bytesPerPixel_);
        }

        // Set pixel format
        if (png_get_color_type(png, info) == PNG_COLOR_TYPE_RGB)
        {
            bytesPerPixel_ = 3;
        }
        else if (png_get_color_type(png, info) == PNG_COLOR_TYPE_RGBA)
        {
            bytesPerPixel_ = 4;
        }
        else
        {
            LOGE("Cannot open file %.!", filename().c_str());
            throw Exception("libpng unsupported color type.");
        }
    }

    void read(png_structp fileHandlePointer, png_bytep data, png_size_t length)
    {
        FileHandle* fileHandle = reinterpret_cast<FileHandle*>(png_get_io_ptr(fileHandlePointer));
        fileHandle->read(reinterpret_cast<char*>(data), length);
    }
}

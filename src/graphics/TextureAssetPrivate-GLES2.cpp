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
#include "w/base/UniquePointer.hpp"
#include <w/base/Log.hpp>
#include <w/base/Exception.hpp>
#include <w/base/ResourceManager.hpp>
#include <w/math/Math.hpp>
#include <memory>

namespace w
{
    namespace graphics
    {
        // Callback function for libpng
        static void read(png_structp p, png_bytep data, png_size_t length);

        TextureAssetPrivate::TextureAssetPrivate(const std::string& file, TextureAsset::Clamp::Enum clamp, bool bundledFile):
            Resource(file),
            bytesPerPixel_(0),
            width_(0),
            height_(0),
            xUsage_(0.0f),
            yUsage_(0.0f),
            sourceBitmapWidth_(0),
            sourceBitmapHeight_(0),
            clamp_(clamp),
            tmpData_(NULL),
            textureId_(0)
        {
            loadFileData(bundledFile);
        }
        
        TextureAssetPrivate::TextureAssetPrivate(const w::graphics::Bitmap& bitmap, TextureAsset::Clamp::Enum clamp):
            Resource("bitmap"),
            bytesPerPixel_(0),
            width_(0),
            height_(0),
            xUsage_(0.0f),
            yUsage_(0.0f),
            sourceBitmapWidth_(0),
            sourceBitmapHeight_(0),
            clamp_(clamp),
            tmpData_(NULL),
            textureId_(0)
        {
            loadBitmap(bitmap);
        }

        TextureAssetPrivate::~TextureAssetPrivate()
        {
            if(tmpData_ != NULL)
            {
                delete [] tmpData_;
                tmpData_ = NULL;
            }

            if(textureId_ != 0)
            {
                glDeleteTextures(1, &textureId_);
                textureId_ = 0;
            }
        }

        unsigned int TextureAssetPrivate::width() const
        {
            return width_;
        }

        unsigned int TextureAssetPrivate::height() const
        {
            return height_;
        }

        void TextureAssetPrivate::loadFileData(bool bundledFile)
        {
            static const int flags = PNG_TRANSFORM_STRIP_16 |
                PNG_TRANSFORM_GRAY_TO_RGB | PNG_TRANSFORM_PACKING |
                PNG_TRANSFORM_EXPAND;

            // Create libpng structs
            png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
            if(!png)
            {
                LOGE("Cannot open file %.!", filename().c_str());
                throw Exception("Couldn't create libpng read struct.");
            }

            png_infop info = png_create_info_struct(png);
            if(!info)
            {
                LOGE("Cannot open file %.!", filename().c_str());
                throw Exception("Couldn't create libpng info struct.");
            }

            // Errors, TODO: check if this is needed.
            if(setjmp(png_jmpbuf(png)))
            {
                LOGE("libpng error while reading file %s.", filename().c_str());
                throw Exception("Could not load png file!");
            }

            // Start reading the file
            ReferencedPointer<FileHandle> fileHandle;
            if(bundledFile)
            {
                fileHandle = ResourceManager::bundledFile(filename());
            }
            else
            {
                fileHandle = ResourceManager::dynamicFileForRead(filename());
            }
            
            // Read the image header and data
            png_set_read_fn(png, reinterpret_cast<void*>(fileHandle.pointer()), read);
            png_read_png(png, info, flags, 0);
            png_bytepp rows = png_get_rows(png, info);

            // Set pixel format
            if(png_get_color_type(png, info) == PNG_COLOR_TYPE_RGB)
            {
                bytesPerPixel_ = 3;
            }
            else if(png_get_color_type(png, info) == PNG_COLOR_TYPE_RGBA)
            {
                bytesPerPixel_ = 4;
            }
            else
            {
                LOGE("Cannot open file %.!", filename().c_str());
                throw Exception("libpng unsupported color type.");
            }

            // Copy to continous memory
            sourceBitmapWidth_ = png_get_image_width(png, info);
            sourceBitmapHeight_ = png_get_image_height(png, info);
            width_ = math::nextPowerOfTwo(sourceBitmapWidth_);
            height_ = math::nextPowerOfTwo(sourceBitmapHeight_);
            xUsage_ = (float)sourceBitmapWidth_ / (float)width_;
            yUsage_ = (float)sourceBitmapHeight_ / (float)height_;
            unsigned int dataSize = width_ * height_ * bytesPerPixel_;
            tmpData_ = new char[dataSize];
            for(unsigned int i = 0; i < dataSize; i++)
            {
                tmpData_[i] = 0;
            }
            for(unsigned int i = 0; i < sourceBitmapHeight_; i++)
            {
                memcpy(&(tmpData_)[width_ * bytesPerPixel_ * i], rows[sourceBitmapHeight_ - i - 1], sourceBitmapWidth_ * bytesPerPixel_);
            }

            // Free libpng's struct
            png_destroy_read_struct(&png, &info, 0);
        }

        void TextureAssetPrivate::loadBitmap(const w::graphics::Bitmap& bitmap)
        {
            switch (bitmap.format())
            {
                case w::graphics::Bitmap::Format::LUMINANCE_8:
                    bytesPerPixel_ = 1;
                    break;
                case w::graphics::Bitmap::Format::RGB_888:
                    bytesPerPixel_ = 3;
                    break;
                case w::graphics::Bitmap::Format::RGBA_8888:
                    bytesPerPixel_ = 4;
                    break;
                default:
                    break;
            }
            sourceBitmapWidth_ = bitmap.width();
            sourceBitmapHeight_ = bitmap.height();
            width_ = math::nextPowerOfTwo(sourceBitmapWidth_);
            height_ = math::nextPowerOfTwo(sourceBitmapHeight_);
            xUsage_ = (float)sourceBitmapWidth_ / (float)width_;
            yUsage_ = (float)sourceBitmapHeight_ / (float)height_;
            
            unsigned int dataSize = width_ * height_ * bytesPerPixel_;
            tmpData_ = new char[dataSize];
    
            // TODO: polish copy away
            memcpy(&(tmpData_)[0], bitmap.data(), sourceBitmapWidth_ * sourceBitmapHeight_ * bytesPerPixel_);
            LOGD("CCC CCC CC CCCCCCC CCC CC C CCC     CC%d, %f, %f", sourceBitmapWidth_ * sourceBitmapHeight_ * bytesPerPixel_, xUsage_, yUsage_);
        }

        void TextureAssetPrivate::bind()
        {
            loadGPUData();

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textureId_);
        }

        float TextureAssetPrivate::xUsage() const
        {
            return xUsage_;
        }

        float TextureAssetPrivate::yUsage() const
        {
            return yUsage_;
        }

        unsigned int TextureAssetPrivate::sourceBitmapWidth() const
        {
            return sourceBitmapWidth_;
        }

        unsigned int TextureAssetPrivate::sourceBitmapHeight() const
        {
            return sourceBitmapHeight_;
        }

        void TextureAssetPrivate::loadGPUData()
        {
            if (tmpData_ == NULL)
            {
                return;
            }

            // Create texture
            GLenum format = bytesPerPixel_ == 3 ? GL_RGB : GL_RGBA;
            GLint minFilter = GL_LINEAR_MIPMAP_NEAREST;
            GLint magFilter = GL_LINEAR;

            glGenTextures(1, &textureId_);
            glBindTexture(GL_TEXTURE_2D, textureId_);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

            // Clamp
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, clamp_ & TextureAsset::Clamp::RepeatX ? GL_REPEAT : GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, clamp_ & TextureAsset::Clamp::RepeatY ? GL_REPEAT : GL_CLAMP_TO_EDGE);

            glTexImage2D(GL_TEXTURE_2D,
                0,
                format,
                width_,
                height_,
                0,
                format,
                GL_UNSIGNED_BYTE,
                tmpData_);

            if ((minFilter != GL_NEAREST && minFilter != GL_LINEAR) || (magFilter != GL_NEAREST && magFilter != GL_LINEAR))
            {
                glGenerateMipmap(GL_TEXTURE_2D);
            }
            else
            {
                LOGE("Error in texture filters!\n");
            }

            delete [] tmpData_;
            tmpData_ = NULL;
            // LOGD("TextureAssetPrivate()::loadGPUData(), id:%d, %dx%dx%d", textureId_, width_, height_, bytesPerPixel_);
        }

        void read(png_structp fileHandlePointer, png_bytep data, png_size_t length)
        {
            FileHandle* fileHandle = reinterpret_cast<FileHandle*>(png_get_io_ptr(fileHandlePointer));
            fileHandle->read(reinterpret_cast<char*>(data), length);
        }
    }
}

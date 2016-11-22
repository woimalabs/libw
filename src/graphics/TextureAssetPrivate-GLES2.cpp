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
        unsigned int TextureAssetPrivate::textureIdBinded_ = 0;

        TextureAssetPrivate::TextureAssetPrivate(const std::string& file, TextureAsset::Clamp::Enum clamp, bool bundledFile):
            Resource(file),
            isAssetFile_(true),
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
            isAssetFile_(false),
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
        
        void TextureAssetPrivate::load()
        {
            loadGPUData();
        }

        void TextureAssetPrivate::halfSize()
        {
            char* data = tmpData_;
            unsigned int width = width_;
            unsigned int height = height_;
            unsigned int newWidth = (width_ + 1) / 2;
            unsigned int newHeight = (height_ + 1) / 2;
            char* newData = (char*)malloc(newWidth * newHeight * bytesPerPixel_);
            unsigned int widthXBpp = width_ * bytesPerPixel_;
            unsigned int newWidthXBpp = newWidth * bytesPerPixel_;

            for (unsigned int x = 0; x < newWidth; x++)
            {
                for (unsigned int y = 0; y < newHeight; y++)
                {
                    unsigned int target = y * newWidthXBpp + x * bytesPerPixel_;
                    unsigned int sourceRow1 = (y * 2) * widthXBpp + (x * 2) * bytesPerPixel_;
                    unsigned int sourceRow2 = (y * 2 + 1) * widthXBpp + (x * 2) * bytesPerPixel_;
                    
                    if ((x == (newWidth - 1) && (width % 2 == 1)) && (y == (newHeight - 1) && (height % 2 == 1))) // upper corner case
                    {
                        for (unsigned int colorIndex = 0; colorIndex < bytesPerPixel_; colorIndex++) // r, g, b, a
                        {
                            newData[target + colorIndex] = data[sourceRow1 + colorIndex] / 4;
                        }
                    }
                    else if (x == (newWidth - 1) && (width % 2 == 1)) // right side case
                    {
                        for (unsigned int colorIndex = 0; colorIndex < bytesPerPixel_; colorIndex++) // r, g, b, a
                        {
                            newData[target + colorIndex] =
                                (data[sourceRow1 + colorIndex]
                               + data[sourceRow2 + colorIndex]) / 4;
                        }
                    }
                    else if (y == (newHeight - 1) && (height % 2 == 1)) // top row case
                    {
                        for (unsigned int colorIndex = 0; colorIndex < bytesPerPixel_; colorIndex++) // r, g, b, a
                        {
                            newData[target + colorIndex] =
                                (data[sourceRow1 + colorIndex] + data[sourceRow1 + colorIndex + bytesPerPixel_]) / 4;
                        }
                    }
                    else // "normal" case
                    {
                        for (unsigned int colorIndex = 0; colorIndex < bytesPerPixel_; colorIndex++) // r, g, b, a
                        {
                            newData[target + colorIndex] =
                                (data[sourceRow1 + colorIndex] + data[sourceRow1 + colorIndex + bytesPerPixel_]
                               + data[sourceRow2 + colorIndex] + data[sourceRow2 + colorIndex + bytesPerPixel_]) / 4;
                        }
                    }
                }
            }

            char* tmp = data;
            tmpData_ = newData;
            delete [] tmp;
            width_ = newWidth;
            height_ = newHeight;
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

            /*if(ResourceManager::graphicsDownScale() == 2)
            {
                halfSize();
            }*/

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
    
            for(auto i = 0; i < sourceBitmapHeight_; i++)
            {
                unsigned int offsetTarget = i * width_ * bytesPerPixel_;
                unsigned int offsetSource = i * sourceBitmapWidth_ * bytesPerPixel_;
                memcpy(tmpData_ + offsetTarget, bitmap.data() + offsetSource, sourceBitmapWidth_ * bytesPerPixel_);
            }
        }

        void TextureAssetPrivate::bind()
        {
            loadGPUData();

            static bool activeTexture0Set = false;
            if(!activeTexture0Set)
            {
                glActiveTexture(GL_TEXTURE0);
                activeTexture0Set = true;
            }
            
            if(textureIdBinded_ != textureId_)
            {
                glBindTexture(GL_TEXTURE_2D, textureId_);
                textureIdBinded_ = textureId_;
            }
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
            if(isAssetFile_)
                return sourceBitmapWidth_ * (float)ResourceManager::graphicsDownScale();
            else
                return sourceBitmapWidth_;
        }

        unsigned int TextureAssetPrivate::sourceBitmapHeight() const
        {
            if(isAssetFile_)
                return sourceBitmapHeight_ * (float)ResourceManager::graphicsDownScale();
            else
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
        }

        void read(png_structp fileHandlePointer, png_bytep data, png_size_t length)
        {
            FileHandle* fileHandle = reinterpret_cast<FileHandle*>(png_get_io_ptr(fileHandlePointer));
            fileHandle->read(reinterpret_cast<char*>(data), length);
        }
    }
}

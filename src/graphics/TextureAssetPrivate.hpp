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

#ifndef LIBW_GRAPHICS_TEXTUREASSETPRIVATE
#define LIBW_GRAPHICS_TEXTUREASSETPRIVATE

#include <w/base/Class.hpp>
#include "w/base/Resource.hpp"
#include "w/graphics/TextureAsset.hpp"
#include <png.h>
#ifdef __linux__ // & Android
    #include <GLES2/gl2.h>
#else // APPLE
    #include <OpenGLES/ES2/gl.h>
#endif

namespace w
{
    namespace graphics
    {
        class TextureAssetPrivate: public Resource
        {
        public:
            UNCOPYABLE(TextureAssetPrivate)

            TextureAssetPrivate(const std::string & filename, TextureAsset::Clamp::Enum clamp = TextureAsset::Clamp::ToEdge, bool bundledFile = true);
            ~TextureAssetPrivate();

            unsigned int width() const;
            unsigned int height() const;
            bool hasAlpha() const
            {
                return bytesPerPixel_ == 4;
            }

            void bind();
            float xUsage() const;
            float yUsage() const;
            unsigned int sourceBitmapWidth() const;
            unsigned int sourceBitmapHeight() const;

        private:
            void loadFileData(bool bundledFile);
            void loadGPUData();

            unsigned int bytesPerPixel_;

            unsigned int width_;
            unsigned int height_;
            float xUsage_;
            float yUsage_;
            unsigned int sourceBitmapWidth_;
            unsigned int sourceBitmapHeight_;
            TextureAsset::Clamp::Enum clamp_;

            // Pointer where loaded data array is kept
            Mutex mutex_;
            char* tmpData_;

            // Android, linux, iOS. All use GLES2
            GLuint textureId_;
        };
    }
}

#endif


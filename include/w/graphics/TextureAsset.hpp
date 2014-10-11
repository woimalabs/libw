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

#ifndef LIBW_GRAPHICS_TEXTUREASSET
#define LIBW_GRAPHICS_TEXTUREASSET

#include <w/base/Class.hpp>
#include <string>

namespace w
{
    namespace graphics
    {
        class TextureAsset
        {
        public:
            COPYABLE(TextureAsset)

            struct Clamp
            {
                enum Enum
                {
                    ToEdge = 0,
                    RepeatX = 1 << 0,
                    RepeatY = 1 << 1,
                    Repeat = RepeatX | RepeatY
                };
            };

            /**
             * Creates shader program.
             *
             * @note This class can load it self even without GL context. For
             * drawing GL context is needed.
             *
             * @note If non power of two image data is loaded the texture will be
             * created with up to next power of two size. And upper left corner of
             * the area will be filled.
             *
             * @param [in]  filename    Texture filename to load
             */
            TextureAsset(const std::string& filename, Clamp::Enum clamp = Clamp::ToEdge);
            ~TextureAsset();

            unsigned int width() const;
            unsigned int height() const;
            bool hasAlpha() const;
            float xUsage() const;
            float yUsage() const;
            unsigned int sourceBitmapWidth() const;
            unsigned int sourceBitmapHeight() const;

        private:
            friend class RendererPrivate;
            class TextureAssetPrivate* private_;
        };
    }
}

#endif

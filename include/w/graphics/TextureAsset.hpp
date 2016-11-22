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
#include <w/graphics/Bitmap.hpp>
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

            struct MagFilter
            {
                enum Enum
                {
                    Linear = 1 << 0,
                    Nearest = 1 << 1
                };
            };

            struct MinFilter
            {
                enum Enum
                {
                    Linear = 1 << 0,
                    Nearest = 1 << 1,
                    LinearMipMapLinear = 1 << 2,
                    LinearMipMapNearest = 1 << 3,
                    NearestMipMapLinear = 1 << 4,
                    NearestMipMapNearest = 1 << 5
                };
            };

            /**
             * Creates texture asset from given file.
             *
             * @param [in]  filename    Texture filename to load
             * @param [in]  clamp       Repeat on texture edges
             * @param [in]  bundledFile Is the file given a bundled or a dynamic file.
             */
            TextureAsset(const std::string& filename, Clamp::Enum clamp = Clamp::ToEdge, bool bundledFile = true);
            
            /**
             * Creates texture asset from bitmap.
             *
             * @param [in]  filename    Texture filename to load
             * @param [in]  clamp       Repeat on texture edges
             */
            TextureAsset(const w::graphics::Bitmap& bitmap, Clamp::Enum clamp = Clamp::ToEdge);
            ~TextureAsset();

            bool hasAlpha() const;
            float xUsage() const;
            float yUsage() const;
            unsigned int sourceBitmapWidth() const;
            unsigned int sourceBitmapHeight() const;

            friend class BatchableRectangleMesh;
            friend class BatchableRectangleMeshAtlas;
            unsigned int width() const;
            unsigned int height() const;
            
            /*
             * Force load to GPU memory. Otherwise loading happens when Renderer uses this asset.
             */
            void load();
            
        private:
            friend class RendererPrivate;
            class TextureAssetPrivate* private_;
        };
    }
}

#endif

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

#include "w/graphics/TextureAsset.hpp"
#include "TextureAssetPrivate.hpp"
#include "w/base/ResourceManagerPrivate.hpp"
#include "w/base/Exception.hpp"

namespace w
{
    namespace graphics
    {
        std::string clampForId(TextureAsset::Clamp::Enum clamp)
        {
            return std::string("clamp" +
                (clamp & TextureAsset::Clamp::RepeatX) ? "X1" : "X0" +
                (clamp & TextureAsset::Clamp::RepeatY) ? "Y1" : "Y0"
                );
        }

        TextureAsset::TextureAsset(const std::string& filename, Clamp::Enum clamp):
            private_(NULL)
        {
            std::string id = "Texture:" + filename + ".clamp:" + clampForId(clamp);
            private_ = dynamic_cast<TextureAssetPrivate*>(ResourceManagerPrivate::assetPrivate(id));
            if (private_ == NULL)
            {
                private_ = new TextureAssetPrivate(filename, clamp);
                ResourceManagerPrivate::setAssetPrivate(id, private_);
            }

            private_->increment();
        }

        TextureAsset::TextureAsset(TextureAsset const& r):
            private_(r.private_)
        {
            if (private_ != NULL)
            {
                private_->increment();
            }
            else
            {
                LOGE("TextureAsset having NULL private, internal ERROR!");
            }
        }

        TextureAsset::~TextureAsset()
        {
            private_->decrement();
        }

        TextureAsset& TextureAsset::operator=(TextureAsset const& r)
        {
            if (this != &r)
            {
                // If privates differ-> we can decrement our private
                if (private_ != r.private_)
                {
                    if (private_ != NULL)
                    {
                        private_->decrement();
                        private_ = NULL;
                    }
                }
                
                // Assign r instance if it's other than NULL. NULL is our initial value.
                if (r.private_ != NULL)
                {
                    private_ = r.private_;
                    private_->increment();
                }
                else
                {
                    LOGE("TextureAsset having NULL private, internal ERROR!");
                }
            }
            return *this;
        }

        unsigned int TextureAsset::width() const
        {
            return private_->width();
        }

        unsigned int TextureAsset::height() const
        {
            return private_->height();
        }

        float TextureAsset::xUsage() const
        {
            return private_->xUsage();
        }

        float TextureAsset::yUsage() const
        {
            return private_->yUsage();
        }

        unsigned int TextureAsset::sourceBitmapWidth() const
        {
            return private_->sourceBitmapWidth();
        }

        unsigned int TextureAsset::sourceBitmapHeight() const
        {
            return private_->sourceBitmapHeight();
        }

        bool TextureAsset::hasAlpha() const
        {
            return private_->hasAlpha();
        }
    }
}

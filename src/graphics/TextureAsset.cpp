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

namespace w
{
    TextureAsset::TextureAsset(const std::string& filename):
        private_(NULL)
    {
        std::string id = "Texture:" + filename;
        private_ = dynamic_cast<TextureAssetPrivate*>(ResourceManagerPrivate::assetPrivate(id));
        if (private_ == NULL)
        {
            private_ = new TextureAssetPrivate(filename);
            ResourceManagerPrivate::setAssetPrivate(id, private_);
        }

        private_->increment();
    }

    TextureAsset::TextureAsset(TextureAsset const& r):
        private_(r.private_)
    {
        private_->increment();
    }

    TextureAsset::~TextureAsset()
    {
        private_->decrement();
    }

    TextureAsset& TextureAsset::operator=(TextureAsset const& r)
    {
        if (this != &r)
        {
            private_ = r.private_;
            private_->increment();
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

    bool TextureAsset::hasAlpha() const
    {
        return private_->hasAlpha();
    }
}

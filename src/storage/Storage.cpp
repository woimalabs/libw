/**
 * libw
 *
 * Copyright (C) 2012-2014 Woima Solutions
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

#include "w/storage/Storage.hpp"
#include "StoragePrivate.hpp"
#include <w/base/Exception.hpp>

namespace w
{
    Storage::Storage(std::string const& id):
        private_(new StoragePrivate(id))
    {            
        if (private_ != NULL)
        {
            private_->increment();
        }
        else
        {
            LOGE("storage having NULL private, internal ERROR!");
        }
    }

    Storage::~Storage()
    {
        private_->decrement();
    }

    Storage::Storage(Storage const& r):
        private_(r.private_)
    {
        if (private_ != NULL)
        {
            private_->increment();
        }
        else
        {
            LOGE("Storage having NULL private, internal ERROR!");
        }
    }

    Storage& Storage::operator=(Storage const& r)
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
                LOGE("Storage having NULL private, internal ERROR!");
            }
        }
        return *this;
    }

    bool Storage::hasInt(const std::string& id)
    {
        return private_->hasInt(id);
    }

    void Storage::setInt(const std::string& id, int value)
    {
        private_->setInt(id, value);
        private_->save();
    }

    int Storage::getInt(const std::string& id)
    {
        return private_->getInt(id);
    }

    bool Storage::hasString(const std::string& id)
    {
        return private_->hasString(id);
    }

    void Storage::setString(const std::string& id, const std::string& value)
    {
        private_->setString(id, value);
        private_->save();
    }

    std::string Storage::getString(const std::string& id)
    {
        return private_->getString(id);
    }

    std::string Storage::getString(const std::string& id, const std::string& defaultValue)
    {
        return private_->getString(id, defaultValue);
    }
    
    void Storage::remove(const std::string &key)
    {
        private_->remove(key);
    }
    
    void Storage::serializeItem(const std::string& keyToSerialize, std::string& target)
    {
        private_->serializeItem(keyToSerialize, target);
    }
}

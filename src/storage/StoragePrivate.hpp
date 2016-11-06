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

#ifndef LIBW_STORAGE_STORAGEPRIVATE
#define LIBW_STORAGE_STORAGEPRIVATE

#include "StorageItem.hpp"
#include "w/base/Referenced.hpp"
#include "w/base/Class.hpp"
#include <string>
#include <list>
#include <vector>

namespace w
{
    class StoragePrivate: public Referenced
    {
    public:
        UNCOPYABLE(StoragePrivate)

        static const std::string BlockHeaderStart;
        static const std::string BlockHeaderEnd;
        static const std::string BlockDataStart;
        static const std::string BlockDataEnd;
        static const char BlockLineChange;

        StoragePrivate(const std::string& name);
        ~StoragePrivate();
        bool hasInt(const std::string& key);
        void setInt(const std::string& key, int value);
        int getInt(const std::string& key);
        bool hasString(const std::string& key);
        void setString(const std::string& key, const std::string& value);
        std::string getString(const std::string& key);
        std::string getString(const std::string& key, const std::string& defaultValue);
        void remove(const std::string& key);
        void load();
        void save();
        void serializeItem(const std::string& keyToSerialize, std::string& target);

    private:
        bool has(StorageItem::Type::Enum type, const std::string& key);
        void loadFile(char** target, unsigned int& length);
        void saveFile(const char* data, unsigned int length);
        std::string filePath();
        std::string serialize();
        std::list<StorageItem*> list_;
        std::string name_;
    };
}

#endif

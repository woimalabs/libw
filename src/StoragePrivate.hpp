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

#ifndef LIBW_STORAGEPRIVATE
#define LIBW_STORAGEPRIVATE

#include "StorageItem.hpp"
#include <string>
#include <list>

namespace w
{
    class StoragePrivate
    {
    public:
        static const std::string BlockHeaderStart;
        static const std::string BlockHeaderEnd;

        StoragePrivate(const std::string& id);
        ~StoragePrivate();
        bool hasInt(const std::string& key);
        void setInt(const std::string& key, int value);
        int getInt(const std::string& key);
        void load();
        void save();

    private:
        bool has(StorageItem::Type::Enum type, const std::string& key);
        void loadFile(char** target, unsigned int& length);
        void saveFile(const char* data, unsigned int length);
        std::string filePath();
        std::string serialize();
        std::list<StorageItem*> list_;
        std::string id_;
    };
}

#endif

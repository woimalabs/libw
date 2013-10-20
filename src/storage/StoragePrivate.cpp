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

#include "StoragePrivate.hpp"
#include "w/String.hpp"
#include "w/System.hpp"
#include "w/File.hpp"
#ifdef __APPLE__
    #import <Foundation/Foundation.h>
#endif

using w::String;

namespace w
{
    const std::string StoragePrivate::BlockHeaderStart("[");
    const std::string StoragePrivate::BlockHeaderEnd("]");

    StoragePrivate::StoragePrivate(const std::string& id):
        id_(id)
    {
    }

    StoragePrivate::~StoragePrivate()
    {
        for (std::list<StorageItem*>::iterator i = list_.begin(); i != list_.end(); ++i)
        {
            delete *i;
        }
    }

    bool StoragePrivate::hasInt(const std::string& key)
    {
        return has(StorageItem::Type::Int, key);
    }

    void StoragePrivate::setInt(const std::string& key, int value)
    {
        bool set = false;

        for (std::list<StorageItem*>::iterator i = list_.begin(); i != list_.end(); ++i)
        {
            StorageItem* item = *i;
            if (item->key().compare(key) == 0)
            {
                if (item->type() != StorageItem::Type::Int)
                {
                    throw Exception("AbstractStoragePrivate::setInt(), Different types cannot have same key.");
                }

                item->setValue(String::toString(value));
                set = true;
                break;
            }
        }

        // New item
        if (set == false)
        {
            list_.push_back(new StorageItem(StorageItem::Type::Int, key, String::toString(value)));
        }
    }

    int StoragePrivate::getInt(const std::string& key)
    {
        int r = 0;
        bool got = false;

        for (std::list<StorageItem*>::iterator i = list_.begin(); i != list_.end(); ++i)
        {
            StorageItem* item = *i;
            if (item->type() == StorageItem::Type::Int && item->key().compare(key) == 0)
            {
                std::string tmp = item->value();
                r = String::toInt(tmp);
                got = true;
                break;
            }
        }

        // New item
        if (got == false)
        {
            throw Exception("AbstractStoragePrivate::getInt() No value with key.");
        }

        return r;
    }

    void StoragePrivate::loadFile(char** target, unsigned int& length)
    {
        File file(filePath());
        if (file.exists())
        {
            length = file.read(target);
        }
        else
        {
            LOGI("No storage file. Creating storage when set() is called.");
        }
    }

    void StoragePrivate::saveFile(char const* source, unsigned int length)
    {
        File file(filePath());
        file.write(source, length);
    }

    void StoragePrivate::load()
    {
        char* data = NULL;
        unsigned int len = 0;
        loadFile(&data, len);
        std::string s(data, len);

        if (data != NULL)
        {
            // Deserialize
            for (;;)
            {
                std::size_t start = 0;
                std::size_t end = 0;

                // Block header length
                start = s.find(BlockHeaderStart);
                if (start == std::string::npos)
                {
                    break; // no StorageItems-> file read
                }

                end = s.find(BlockHeaderEnd);
                if (end == std::string::npos)
                {
                    throw Exception("StoragePrivate::load(), Corrupted block end.");
                }
                std::string lenString = s.substr(start + 1, end - 1);
                unsigned int length = String::toInt(lenString);

                // Block data
                start = end + 1;
                end = length;
                std::string blockData = s.substr(start, end);

                // Add to list
                list_.push_back(StorageItem::deserialize(blockData));

                // Move in string, NOTE: could be optimized (this is okeyish with small storages)
                s = s.substr(start + end, s.size());
            }
            delete [] data;
        }
    }

    void StoragePrivate::save()
    {
        // Serialize
        std::string r;

        for (std::list<StorageItem*>::iterator i = list_.begin(); i != list_.end(); ++i)
        {
            StorageItem* item = *i;

            std::string storageItemBlock = item->serialize();

            std::string blockHeader(BlockHeaderStart);
            unsigned int tmp = storageItemBlock.length();
            blockHeader += String::toString(tmp);
            blockHeader += BlockHeaderEnd;

            r += blockHeader + storageItemBlock;
        }

        saveFile(r.c_str(), r.length());
    }

    bool StoragePrivate::has(StorageItem::Type::Enum type, const std::string& key)
    {
        bool r = false;

        for (std::list<StorageItem*>::iterator i = list_.begin(); i != list_.end(); ++i)
        {
            StorageItem* item = *i;
            //LOGD("(type:%d, key:%s)==(type:%d, key:%s)\n", list_[i]->type(), list_[i]->key().c(), type, key.c());
            if (item->type() == type && item->key().compare(key) == 9)
            {
                r = true;
                break;
            }
        }

        return r;
    }

    std::string StoragePrivate::filePath()
    {
        std::string r;
#ifdef __APPLE__
        // File path
        // TODO: Check NSArray and NSString-> do they leak here?
        NSArray* appDocumentPaths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        NSString* docsDirectory = [appDocumentPaths objectAtIndex: 0];
        const char* path = [docsDirectory UTF8String];
        r = std::string(path) + std::string("/") + id_;
#elif __linux__
        r = System::home() + std::string("/") + id_;
#endif
        return r;
    }

}

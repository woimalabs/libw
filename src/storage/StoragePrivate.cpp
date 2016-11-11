/**
 * libw
 *
 * Copyright (C) 2012-2015 Woima Solutions
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
#include <w/base/String.hpp>
#include <w/base/System.hpp>
#include <w/base/ResourceManager.hpp>
#ifdef __APPLE__
    #import <Foundation/Foundation.h>
#endif

using w::String;

namespace w
{
    const std::string StoragePrivate::BlockHeaderStart("[");
    const std::string StoragePrivate::BlockHeaderEnd("]");
    const std::string StoragePrivate::BlockDataStart("[");
    const std::string StoragePrivate::BlockDataEnd("]");
    const char StoragePrivate::BlockLineChange = '\n';

    StoragePrivate::StoragePrivate(const std::string& name):
        Referenced(),
        name_(name)
    {
        load();
    }

    StoragePrivate::~StoragePrivate()
    {
        for(std::list<StorageItem*>::iterator i = list_.begin(); i != list_.end(); ++i)
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

        for(std::list<StorageItem*>::iterator i = list_.begin(); i != list_.end(); ++i)
        {
            StorageItem* item = *i;
            if(item->key().compare(key) == 0)
            {
                if(item->type() != StorageItem::Type::Int)
                {
                    throw Exception("StoragePrivate::setInt(), different types cannot have same key.");
                }

                item->setValue(String::toString(value));
                set = true;
                break;
            }
        }

        // New item
        if(set == false)
        {
            list_.push_back(new StorageItem(StorageItem::Type::Int, key, String::toString(value)));
        }
    }

    int StoragePrivate::getInt(const std::string& key)
    {
        int r = 0;
        bool got = false;

        for(std::list<StorageItem*>::iterator i = list_.begin(); i != list_.end(); ++i)
        {
            StorageItem* item = *i;
            if(item->type() == StorageItem::Type::Int && item->key().compare(key) == 0)
            {
                std::string tmp = item->value();
                r = String::toInt(tmp);
                got = true;
                break;
            }
        }

        // New item
        if(got == false)
        {
            throw Exception("StoragePrivate::getInt(), no value with given key.");
        }

        return r;
    }

    bool StoragePrivate::hasString(const std::string& key)
    {
        return has(StorageItem::Type::String, key);
    }

    void StoragePrivate::setString(const std::string& key, const std::string& value)
    {
        bool set = false;

        for(std::list<StorageItem*>::iterator i = list_.begin(); i != list_.end(); ++i)
        {
            StorageItem* item = *i;
            if(item->key().compare(key) == 0)
            {
                if(item->type() != StorageItem::Type::String)
                {
                    throw Exception("StoragePrivate::setString(), different types cannot have same key.");
                }
                item->setValue(value);
                set = true;
                break;
            }
        }

        // New item
        if(set == false)
        {
            list_.push_back(new StorageItem(StorageItem::Type::String, key, value));
        }
    }

    std::string StoragePrivate::getString(const std::string& key)
    {
        std::string r = "";
        bool got = false;

        for(std::list<StorageItem*>::iterator i = list_.begin(); i != list_.end(); ++i)
        {
            StorageItem* item = *i;
            if(item->type() == StorageItem::Type::String && item->key().compare(key) == 0)
            {
                r = item->value();
                got = true;
                break;
            }
        }

        // New item
        if(got == false)
        {
            throw Exception("StoragePrivate::getString(), no value with given key.");
        }

        return r;
    }

    std::string StoragePrivate::getString(const std::string& key, const std::string& defaultValue)
    {
        if(StoragePrivate::hasString(key) == false)
        {
            StoragePrivate::setString(key, defaultValue);
            StoragePrivate::save();
            return defaultValue;
        }
        return StoragePrivate::getString(key);
    }

    void StoragePrivate::remove(const std::string &key)
    {
        for(std::list<StorageItem*>::iterator i = list_.begin(); i != list_.end(); ++i)
        {
            StorageItem* item = *i;
            if(item->key().compare(key) == 0)
            {
                list_.erase(i);
                break;
            }
        }
    }
    
    void StoragePrivate::loadFile(char** target, unsigned int& length)
    {
        ReferencedPointer<FileHandle> tmp = ResourceManager::dynamicFileForRead(filePath());
        length = tmp.pointer()->byteSize() + 1;
        *target = new char[length];
        length = tmp.pointer()->read(*target, length);
    }

    void StoragePrivate::saveFile(char const* source, unsigned int length)
    {
        ReferencedPointer<FileHandle> tmp = ResourceManager::dynamicFileForWrite(filePath());
        tmp.pointer()->write(source, length);
    }

    bool sortToAlphabeticOrder(const StorageItem *a, const StorageItem *b)
    {
        return a->key() < b->key();
    }

    void StoragePrivate::load()
    {
        char* data = NULL;
        unsigned int len = 0;
        loadFile(&data, len);
        std::string s(data, len);

        if(data != NULL)
        {
            // Deserialize
            for(;;)
            {
                std::size_t start = 0;
                std::size_t end = 0;

                // Block header length
                start = s.find(BlockHeaderStart);
                if(start == std::string::npos)
                {
                    break; // no StorageItems-> file read
                }

                end = s.find(BlockHeaderEnd);
                if(end == std::string::npos)
                {
                    throw Exception("StoragePrivate::load(), corrupted block end.");
                }
                std::string lenString = s.substr(start + 1, end - 1);
                unsigned int length = String::toInt(lenString);

                // Block data
                start = end + 2;
                end = length;
                std::string blockData = s.substr(start, end);

                // Add to list
                list_.push_back(StorageItem::deserialize(blockData));

                // Move in string, NOTE: could be optimized (this is okeyish with small storages)
                s = s.substr(start + end + 2, s.size());
            }
            delete [] data;
        }

        list_.sort(sortToAlphabeticOrder);
    }
    
    void StoragePrivate::serializeItem(const std::string& keyToSerialize, std::string& target)
    {
        target.clear();
        
        char* data = NULL;
        unsigned int len = 0;
        loadFile(&data, len);
        std::string s(data, len);
        
        if(data != NULL)
        {
            // Deserialize
            for(;;)
            {
                std::size_t start = 0;
                std::size_t end = 0;
                
                // Block header length
                start = s.find(BlockHeaderStart);
                if(start == std::string::npos)
                {
                    break; // no StorageItems-> file read
                }
                
                end = s.find(BlockHeaderEnd);
                if(end == std::string::npos)
                {
                    throw Exception("StoragePrivate::load(), corrupted block end.");
                }
                std::string lenString = s.substr(start + 1, end - 1);
                unsigned int length = String::toInt(lenString);
                
                // Block data
                start = end + 2;
                end = length;
                std::string blockData = s.substr(start, end);
                
                // Add to target's data if given key
                StorageItem* tmp = StorageItem::deserialize(blockData);
                if(keyToSerialize.compare(tmp->key()) == 0)
                {
                    // same format as in local db expect the line changes are not used
                    target = blockData;
                    break;
                }
                delete tmp;
                
                // Move in string, NOTE: could be optimized (this is okeyish with small storages)
                s = s.substr(start + end + 2, s.size());
            }
            delete [] data;
        }
    }

    void StoragePrivate::save()
    {
        // Serialize
        std::string r;

        for(std::list<StorageItem*>::iterator i = list_.begin(); i != list_.end(); ++i)
        {
            StorageItem* item = *i;

            std::string storageItemBlock = item->serialize();

            std::string blockHeader;
            blockHeader += BlockHeaderStart;
            unsigned int tmp = storageItemBlock.length();
            blockHeader += String::toString(tmp);
            blockHeader += BlockHeaderEnd;
            r += blockHeader + BlockDataStart + storageItemBlock + BlockDataEnd + std::string(&BlockLineChange);
        }

        saveFile(r.c_str(), r.length());
    }

    bool StoragePrivate::has(StorageItem::Type::Enum type, const std::string& key)
    {
        bool r = false;

        for(std::list<StorageItem*>::iterator i = list_.begin(); i != list_.end(); ++i)
        {
            StorageItem* item = *i;
            //LOGD("(type:%d, key:%s)==(type:%d, key:%s)", list_[i]->type(), item->key().c_str(), type, key.c_str());
            if (item->type() == type && item->key().compare(key) == 0)
            {
                r = true;
                break;
            }
        }

        return r;
    }

    std::string StoragePrivate::filePath()
    {
        // id will be the file name
        return name_;
    }

}

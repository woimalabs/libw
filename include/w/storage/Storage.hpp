/**
 * libw
 *
 * Copyright (C) 2012-2016 Woima Solutions
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

#ifndef LIBW_STORAGE_STORAGE
#define LIBW_STORAGE_STORAGE

#include <string>
#include <vector>
#include <w/base/Class.hpp>

namespace w
{
    class Storage
    {
    public:
        COPYABLE(Storage);

        /**
         * Creates storage with given name.
         *
         * @param [in]  name    Storage name to be set.
         */
        Storage(const std::string& name);
        ~Storage();

        /**
         * Checks if storage has given key.
         *
         * @param [in]  key     Key name
         * @return              TRUE if storage has int with given key. FALSE otherwise.
         */
        bool hasInt(const std::string& key);

        /**
         * Sets new value to int.
         * @note Storage will save itself during this function call.
         *
         * @param [in]  key     Key to set.
         * @param [in]  value   New value to set to the key.
         */
        void setInt(const std::string& key, int value);

        /**
         * Gets value with given key.
         * @note Use hasInt() to check that storage has the given key!
         *
         * @param [in]  key     Key to get.
         * @return              Value of the key.
         */
        int getInt(const std::string& key);

        /**
         * Checks if storage has given key.
         *
         * @param [in]  key     Key name
         * @return              TRUE if storage has int with given key. FALSE otherwise.
         */
        bool hasString(const std::string& key);

        /**
         * Sets new value to int.
         * @note Storage will save itself during this function call.
         *
         * @param [in]  key     Key to set.
         * @param [in]  value   New value to set to the key.
         */
        void setString(const std::string& key, const std::string& value);

        /**
         * Gets value with given key.
         * @note Use hasInt() to check that storage has the given key!
         *
         * @param [in]  key     Key to get.
         * @return              Value of the key.
         */
        std::string getString(const std::string& key);

        /**
         * Gets value with given key. Sets given value for key if key not found
         * and saves the storage file.
         *
         * @param [in]  key             Key to get.
         * @param [in]  defaultValue    Default value set if no key was present.
         * @return                      Value of the key.
         */
        std::string getString(const std::string& key, const std::string& defaultValue);
        
        /**
         * Removed item with given key. If key does not exist - nothing is done.
         *
         * @param [in]  key             Key to get.
         */
        void remove(const std::string& key);

        /**
         * Serializes data from local storage.
         *
         * @param [in]  keyToSerialize  Key to be serialized from local storage if found.
         * @param [out] target          String to hold the serialized data.
         */
        void serializeItem(const std::string& key, std::string& target);

    private:
        class StoragePrivate* private_;
    };
}

#endif

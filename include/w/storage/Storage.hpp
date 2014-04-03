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

#ifndef LIBW_STORAGE_STORAGE
#define LIBW_STORAGE_STORAGE

#include <string>
#include <w/base/Class.hpp>

namespace w
{
    class Storage
    {
    public:
        UNCOPYABLE(Storage);

        /**
         * Creates storage with given name.
         *
         * @param [in]  name    Storage name to be set. Use application name to avoid conflicts.
         */
        Storage(const std::string& name);
        ~Storage();

        /**
         * Checks if storage has given key.
         *
         * @param [in]  key     Key name
         * @return              TRUE if storage has int with given key. FALSE otherwise.
         */
        static bool hasInt(const std::string& key);

        /**
         * Sets new value to int.
         * @note Storage will save itself during this function call.
         *
         * @param [in]  key     Key to set.
         * @param [in]  value   New value to set to the key.
         */
        static void setInt(const std::string& key, int value);

        /**
         * Gets value with given key.
         * @note Use hasInt() to check that storage has the given key!
         *
         * @param [in]  key     Key to get.
         * @return              Value of the key.
         */
        static int getInt(const std::string& key);

    private:
        static class StoragePrivate* singleton_;
    };
}

#endif

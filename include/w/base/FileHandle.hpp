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

#ifndef LIBW_FILEHANDLE
#define LIBW_FILEHANDLE

#include "w/base/Referenced.hpp"
#include "w/base/Exception.hpp"
#include <stdio.h>
#include <unistd.h>
#include <string>
#ifdef ANDROID
    #include <jni.h>
    #include <sys/types.h>
    #include <android/asset_manager.h>
    #include <android/asset_manager_jni.h>
#endif

namespace w
{
    class FileHandle: public Referenced
    {
    public:
        struct Type
        {
            enum Enum
            {
                ReadOnly_ExceptionIfNotExisting = 1,
                ReadOnly_CreateIfNotExisting = 2,
                WriteOnly_DestroyOldContent_CreateNewIfNotExisting = 3
            };
        };

        ~FileHandle();
        unsigned int read(char* targetBuffer, unsigned int byteAmountToRead);
        unsigned int write(const char* sourceBuffer, unsigned int byteAmountToWrite);
        std::string filename();
        unsigned int byteSize();
        #ifdef ANDROID
            AAsset* pointer();
        #else // linux or iOS
            FILE* pointer();
        #endif
        static bool exists(const std::string & fullPath);

    private:
        friend class ResourceManagerPrivate;
        #ifdef ANDROID
            FileHandle(const std::string& filename, Type::Enum type = Type::ReadOnly_ExceptionIfNotExisting, AAssetManager* androidAssetManager);
        #else // linux or iOS
            FileHandle(const std::string& filename, Type::Enum type = Type::ReadOnly_ExceptionIfNotExisting);
        #endif

        Type::Enum type_;

        void open();
        void close();
        std::string filename_;
        unsigned int currentReadIndex_;
        unsigned int byteSize_;
        #ifdef ANDROID
            AAssetManager* androidAssetManager_;
            AAsset* file_;
        #else // linux or iOS
            FILE *file_;
        #endif
    };
}

#endif

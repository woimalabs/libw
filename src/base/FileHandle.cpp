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

#include "w/base/FileHandle.hpp"
#include "w/base/Exception.hpp"
#include "w/base/Log.hpp"
#include <stdio.h>
#include <unistd.h>
#include <string>

namespace w
{
    #ifdef ANDROID
        FileHandle::FileHandle(const std::string& filename, Type::Enum type, AAssetManager* androidAssetManager):
    #else // linux
        FileHandle::FileHandle(const std::string& filename, Type::Enum type):
    #endif
        type_(type),
        filename_(filename),
        currentReadIndex_(0),
        #ifdef ANDROID
            androidAssetManager_(androidAssetManager),
        #endif
        byteSize_(0)
    {
        open();
    }

    FileHandle::~FileHandle()
    {
        close();
    }

    // Opens the file and defines byte size
    void FileHandle::open()
    {
        // LOGD("opening file: %s", filename_.c_str());

        #ifdef ANDROID
            file_ = AAssetManager_open(androidAssetManager_, filename_.c_str(), AASSET_MODE_STREAMING);
            if (file_ == NULL)
            {
                LOGE("Failed to load filename: '%s'", filename_.c_str());
                throw Exception("FileHandle::open() failed");
            }
            byteSize_ = AAsset_getLength(file_);
        #else // Linux
            const char* tmp = NULL;
            if(type_ == Type::ReadOnly_ExceptionIfNotExisting)
            {
                tmp = "rb";
            }
            else if(type_ == Type::ReadOnly_CreateIfNotExisting)
            {
                tmp = "r";
                FILE* tmpFile = fopen(filename_.c_str(), tmp);
                // need to create the file
                if(tmpFile == NULL)
                {
                    tmpFile = fopen(filename_.c_str(), "w+");
                    if(tmpFile == NULL)
                    {
                        perror(filename_.c_str());
                        LOGE("Failed in creation of filename: '%s'", filename_.c_str());
                        throw Exception("FileHandle::open() failed");
                    }
                    fclose(tmpFile);
                    LOGI("created a new file: %s", filename_.c_str());
                }
                else
                {
                    // file existed, close it and open it again with right parameters
                    fclose(tmpFile);
                    LOGI("no need to create a new file (file existed): %s", filename_.c_str());
                }
                tmp = "rb";
            }
            else if(type_ == Type::WriteOnly_DestroyOldContent_CreateNewIfNotExisting)
            {
                tmp = "wb";
            }
            file_ = fopen(filename_.c_str(), tmp);
            if (file_ == NULL)
            {
                perror(filename_.c_str());
                LOGE("Failed to load filename: '%s'", filename_.c_str());
                throw Exception("FileHandle::open() failed");
            }

            fseek(file_, 0, SEEK_END);
            byteSize_ = ftell(file_);
            rewind (file_);
        #endif
    }

    unsigned int FileHandle::read(char* targetBuffer, unsigned int byteAmountToRead)
    {
        unsigned int bytesRead = 0;
        unsigned int bytesToBeRead = byteAmountToRead;
        while(true)
        {
            #ifdef ANDROID
                unsigned int readAmount = AAsset_read(file_, targetBuffer, bytesToBeRead);
            #else // linux
                size_t readAmount = fread(targetBuffer, 1, bytesToBeRead, file_);
            #endif

            bytesRead += readAmount;
            bytesToBeRead -= readAmount;

            if(readAmount == 0 || bytesToBeRead <= 0)
            {
                break;
            }
        }
        currentReadIndex_ += bytesRead;
        return bytesRead;
    }

    unsigned int FileHandle::write(const char* sourceBuffer, unsigned int byteAmountToWrite)
    {
        return fwrite(sourceBuffer, byteAmountToWrite, 1, file_);
    }

    std::string FileHandle::filename()
    {
        return filename_;
    }

    unsigned int FileHandle::byteSize()
    {
        return byteSize_;
    }

#ifdef ANDROID
    AAsset* FileHandle::pointer()
    {
        return file_;
    }
#else // linux
    FILE* FileHandle::pointer()
    {
        return file_;
    }
#endif

    bool FileHandle::exists(const std::string& fullPath)
    {
        bool r = false;
        if(access(fullPath.c_str(), F_OK) != -1)
        {
            r = true;
        }
        return r;
    }

    void FileHandle::close()
    {
        #ifdef ANDROID
            AAsset_close(file_);
        #else // linux
            fclose(file_);
        #endif
        file_ = NULL;
    }
}

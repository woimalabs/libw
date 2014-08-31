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

#ifndef LIBW_FILE
#define LIBW_FILE

#include "w/base/Exception.hpp"
#include <stdio.h>
#include <unistd.h>
#include <string>

namespace w
{
    class File
    {
    public:
        File(const std::string& fullPath):
            fullPath_(fullPath)
        {
        }

        ~File()
        {
        }

        bool exists()
        {
            bool r = false;

            if (access(fullPath_.c_str(), F_OK) != -1)
            {
                r = true;
            }

            return r;
        }

        unsigned int read(char** result)
        {
            unsigned int size = 0;
            FILE* f = fopen(fullPath_.c_str(), "rb");

            if (f == NULL)
            {
                *result = NULL;
                throw Exception("File::read() Opening failed.");
            }

            fseek(f, 0, SEEK_END);
            size = ftell(f);
            fseek(f, 0, SEEK_SET);
            *result = new char[size + 1];

            if (size != fread(*result, sizeof(char), size, f))
            {
                delete [] *result;
                *result = NULL;
                throw Exception("File::read() Reading failed.");
            }
            fclose(f);

            return size;
        }

        void write(char const* source, unsigned int length)
        {
            FILE* f = fopen(fullPath_.c_str(), "wb+");
            if (!f)
            {
                throw Exception("File::write() Unable to open file!");
            }
            fwrite(source, length, 1, f);
            fclose(f);
        }

    private:
        std::string fullPath_;
    };
}

#endif

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

#ifndef LIBW_STRING
#define LIBW_STRING

#include "w/base/Exception.hpp"
#include "w/base/Log.hpp"
#include <string>
#include <cstring>
#include <vector>
#include <sstream>

namespace w
{
    /**
     * @class String
     *
     * String is a helper class to provide extra functionality related
     * related to std::string.
     */
    class String
    {
    public:
        static std::string toString(unsigned int number)
        {
            std::string r;

            if (number == 0)
            {
                r = std::string("0");
            }
            else
            {
                const char* table = "0123456789";

                // Count length
                unsigned int length = 0;
                unsigned int tmp = number;
                while (tmp > 0)
                {
                    tmp /= 10;
                    length++;
                }

                // Create string
                char* buffer = new char[length + 1];
                int index = length - 1;
                for (int i = index; i >= 0; i--)
                {
                    buffer[i] = table[number % 10];
                    number /= 10;
                }
                buffer[length] = '\0';
                r = std::string(buffer);
                delete [] buffer;
            }

            return r;
        }

        static std::string toString(int number)
        {
            std::string r;

            bool negative = (number < 0);
            if (negative)
            {
                number = -number;
            }

            r = toString((unsigned int)number);

            if (negative)
            {
                r = std::string("-") + r;
            }

            return r;
        }

        static int toInt(std::string const& str)
        {
            int r = std::stoi(str);
            return r;
        }

        static std::vector<std::string> & split(const std::string &s, char delim, std::vector<std::string> &elems)
        {
            std::stringstream ss(s);
            std::string item;
            while(std::getline(ss, item, delim))
            {
                elems.push_back(item);
            }
            return elems;
        }

        static std::vector<std::string> split(const std::string &s, char delim)
        {
            std::vector<std::string> elems;
            split(s, delim, elems);
            return elems;
        }
    };
}

#endif

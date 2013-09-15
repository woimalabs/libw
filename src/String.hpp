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

#ifndef LIBW_STRING
#define LIBW_STRING

#include "w/Exception.hpp"
#include <string>
#include <cstring>

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
                r.insert(0, sizeof(char), '-');
            }

            return r;
        }

        static int toInt(std::string const& str)
        {
            int r = 0;

            int lastIndex = str.length() - 1;
            unsigned int multiplier = 1;

            for (int i = lastIndex; i >= 0; i--)
            {
                if (i == 0 && str.c_str()[i] == '-')
                {
                    r = -r;
                }
                else
                {
                    int tmp = str.c_str()[i] - 48;
                    if (tmp < 0 || tmp > 9)
                    {
                        throw Exception("String::toInt() with non numeric chars.");
                    }
                    r += multiplier * tmp;
                    multiplier *= 10;
                }
            }
            return r;
        }
    };
}

#endif

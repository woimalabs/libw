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

#ifndef LIBW_GRAPHICS_STRIDE
#define LIBW_GRAPHICS_STRIDE

#include <w/base/Log.hpp>
#include <w/base/Exception.hpp>
#include <string>

namespace w
{
    namespace graphics
    {
        struct StrideType
        {
            enum Enum
            {
                UnDefined = 0,
                Float32 = 1
            };
        };

        static unsigned int sizeOf(StrideType::Enum type)
        {
            unsigned int r = 0;
            switch(type)
            {
                case StrideType::Float32:
                {
                    r = 4;
                    break;
                }
                default:
                {
                    LOGE("Unknown StrideType with value:%d", (unsigned int)type);
                    throw Exception("Unknown StrideType");
                    break;
                }
            }
            return r;
        }

        struct StrideComponent
        {
            StrideComponent(const std::string& shaderSymbolName,
                unsigned int byteOffset,
                unsigned int numberOfComponents,
                StrideType::Enum type):

                shaderSymbolName(shaderSymbolName),
                byteOffset(byteOffset),
                numberOfComponents(numberOfComponents),
                type(type)
            {
            }

            std::string shaderSymbolName;
            unsigned int byteOffset;
            unsigned int numberOfComponents;
            StrideType::Enum type;
        };
    }
}

#endif

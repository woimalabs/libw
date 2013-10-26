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

#ifndef LIBW_SHADERPROGRAMASSET
#define LIBW_SHADERPROGRAMASSET

#include <w/Class.hpp>
#include <string>

/**
 * ShaderProgramAsset is a class for shader programs
 *
 * Supported format(s):
 *  - GLES2 vertex and fragment sources loaded from file
 */
namespace w
{
    class ShaderProgramAsset
    {
    public:
        COPYABLE(ShaderProgramAsset)

        /**
         * Creates shader program.
         *
         * @note You must have GL context to create this class!
         *
         * @param [in]  vertexShaderFilename    Vertex shader filename to load
         * @param [in]  fragmentShaderFilename  Fragment shader filename to load
         */
        ShaderProgramAsset(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename);
        ~ShaderProgramAsset();

    private:
        class ShaderProgramAssetPrivate* private_;
    };
}

#endif

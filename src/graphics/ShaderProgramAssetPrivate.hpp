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

#ifndef LIBW_GRAPHICS_SHADERPROGRAMASSETPRIVATE
#define LIBW_GRAPHICS_SHADERPROGRAMASSETPRIVATE

#include <w/base/Referenced.hpp>
#include <w/base/Class.hpp>
#include <w/math/Eigen.hpp>
#ifdef linux
    #include <GLES2/gl2.h>
#else // APPLE
    #include <OpenGLES/ES2/gl.h>
#endif
#include <string>

namespace w
{
    namespace graphics
    {
        class ShaderProgramAssetPrivate: public Referenced
        {
        public:
            UNCOPYABLE(ShaderProgramAssetPrivate)

            // Android, linux, iOS. All use GLES2.
            typedef GLint ShaderId;

            ShaderProgramAssetPrivate(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename);
            ~ShaderProgramAssetPrivate();

            // Android, linux, iOS. All use GLES2.
            ShaderId uniform(const std::string& symbolName);
            ShaderId attribute(const std::string& symbolName);
            void setUniform(const std::string& symbolName, float value);
            void setUniform(const std::string& symbolName, const Eigen::Matrix4f& value);
            void setUniform(const std::string & symbolName, const Eigen::Vector2f & value);
            void setUniform(const std::string & symbolName, const Eigen::Vector3f & value);
            void setUniform(const std::string& symbolName, const Eigen::Vector4f& value);
            void start();
            static void stop();

        private:
            // Android, linux, iOS. All use GLES2.
            GLuint static createShader(GLenum shaderType, const char* pSource);
            GLuint static createProgram(const char* vertexSource, const char* fragmentSource);
            GLuint programId_;
        };
    }
}

#endif

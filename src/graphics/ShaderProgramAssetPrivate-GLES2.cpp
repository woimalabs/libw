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

#include "ShaderProgramAssetPrivate.hpp"
#include "FileHandle.hpp"
#include "ResourceManagerPrivate.hpp"
#include "w/Log.hpp"

namespace w
{
    static void checkGlError(const char* op)
    {
        for (GLint error = glGetError(); error; error = glGetError())
        {
            LOGI("after %s() glError (0x%x)\n", op, error);
        }
    }

    ShaderProgramAssetPrivate::ShaderProgramAssetPrivate(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename):
        Referenced(),
        programId_(0)
    {
        FileHandle* vertexFileHandle = ResourceManagerPrivate::getFileHandle(vertexShaderFilename);
        unsigned int vertexSourceLength = vertexFileHandle->byteSize();
        char* vertexSourceBuffer = new char[vertexSourceLength +1];
        vertexFileHandle->read(vertexSourceBuffer, vertexSourceLength);
        vertexSourceBuffer[vertexSourceLength] = 0;
        delete vertexFileHandle;

        FileHandle* fragmentFileHandle = ResourceManagerPrivate::getFileHandle(fragmentShaderFilename);
        unsigned int fragmentSourceLength = fragmentFileHandle->byteSize();
        char* fragmentSourceBuffer = new char[fragmentSourceLength +1];
        fragmentFileHandle->read(fragmentSourceBuffer, fragmentSourceLength);
        fragmentSourceBuffer[fragmentSourceLength] = 0;
        delete fragmentFileHandle;

        programId_ = createProgram(vertexSourceBuffer, fragmentSourceBuffer);

        delete [] vertexSourceBuffer;
        delete [] fragmentSourceBuffer;
    }

    ShaderProgramAssetPrivate::~ShaderProgramAssetPrivate()
    {
    }

    GLuint ShaderProgramAssetPrivate::createShader(GLenum shaderType, const char* pSource)
    {
        GLuint shader = glCreateShader(shaderType);
        if (shader)
        {
            glShaderSource(shader, 1, &pSource, NULL);
            glCompileShader(shader);
            GLint compiled = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
            if (!compiled)
            {
                GLint infoLen = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
                if (infoLen)
                {
                    char* buf = new char[infoLen];
                    if (buf)
                    {
                        glGetShaderInfoLog(shader, infoLen, NULL, buf);
                        LOGE("Could not compile shader %d: %s", shaderType, buf);
                        delete [] buf;
                    }
                    glDeleteShader(shader);
                    shader = 0;
                }
            }
        }
        return shader;
    }

    GLuint ShaderProgramAssetPrivate::createProgram(const char* vertexSource, const char* fragmentSource)
    {
        GLuint vertexShader = createShader(GL_VERTEX_SHADER, vertexSource);
        if (!vertexShader)
        {
            return 0;
        }

        GLuint pixelShader = createShader(GL_FRAGMENT_SHADER, fragmentSource);
        if (!pixelShader)
        {
            return 0;
        }

        GLuint program = glCreateProgram();
        if (program)
        {
            glAttachShader(program, vertexShader);
            checkGlError("glAttachShader");
            glAttachShader(program, pixelShader);
            checkGlError("glAttachShader");
            glLinkProgram(program);
            GLint linkStatus = GL_FALSE;
            glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
            if (linkStatus != GL_TRUE)
            {
                GLint bufLength = 0;
                glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
                if (bufLength)
                {
                    char* buf = new char[bufLength];
                    if (buf)
                    {
                        glGetProgramInfoLog(program, bufLength, NULL, buf);
                        LOGE("Could not link program:\n%s\n", buf);
                        delete [] buf;
                    }
                }
                glDeleteProgram(program);
                program = 0;
            }
        }
        return program;
    }
}

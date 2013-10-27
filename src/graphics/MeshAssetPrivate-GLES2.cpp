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

#include "MeshAssetPrivate.hpp"

namespace w
{
    MeshAssetPrivate::MeshAssetPrivate(float width, float height, float uStart, float uEnd, float vStart, float vEnd)
    {
        /*
         * Rectangle we create has two triangles:
         *
         * 1_____2
         *  |  /|
         *  | / |
         *  |/__|
         * 0     3
         *
         */

        // Bottom left corner
        float p0x = 0.0f;
        float p0y = 0.0f;
        float p0u = uStart;
        float p0v = vStart;

        // Top left corner
        float p1x = 0.0f;
        float p1y = height;
        float p1u = uStart;
        float p1v = vEnd;

        // Top right corner
        float p2x = width;
        float p2y = height;
        float p2u = uEnd;
        float p2v = vEnd;

        // Bottom right corner
        float p3x = width;
        float p3y = 0.0f;
        float p3u = uEnd;
        float p3v = vStart;

        // z is 0.0f in our rectangle
        GLfloat vertices[] =
        {
            // First triangle
            p0x, p0y, 0.0f,
            p0u, p0v,

            p1x, p1y, 0.0f,
            p1u, p1v,

            p2x, p2y, 0.0f,
            p2u, p2v,

            // Second triangle
            p0x, p0y, 0.0f,
            p0u, p0v,

            p2x, p2y, 0.0f,
            p2u, p2v,

            p3x, p3y, 0.0f,
            p3u, p3v
        };

        glGenBuffers(1, &vbo_);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glBufferData(GL_ARRAY_BUFFER, 30* sizeof(float), vertices, GL_STATIC_DRAW); // 30: 2 triangles * 3 points * (3 position floats + 2 uv floats)
        vertexCount_ = 6; // 6: 2 triangles * 3 points each

        // Set stride format
        StrideComponent xyz(std::string("xyz"), 5, 0, 3, GL_FLOAT);
        StrideComponent uv(std::string("uv"), 5, 3, 2, GL_FLOAT);
        strideComponents_.push_back(xyz);
        strideComponents_.push_back(uv);
    }

    MeshAssetPrivate::~MeshAssetPrivate()
    {
        if (vbo_ != 0)
        {
            glDeleteBuffers(1, &vbo_);
        }
    }

    const std::vector<MeshAssetPrivate::StrideComponent>& MeshAssetPrivate::strideComponents() const
    {
        return strideComponents_;
    }

    void MeshAssetPrivate::bind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    }

    unsigned int MeshAssetPrivate::vertexCount()
    {
       return vertexCount_;
    }
}

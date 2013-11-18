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
#include "w/base/Log.hpp"

namespace w
{
    MeshAssetPrivate::MeshAssetPrivate(float width, float height, float uStart, float uEnd, float vStart, float vEnd):
        vbo_(0),
        tmpVertices_(NULL),
        vertexCount_(0)
    {
        LOGD("w:%f, h:%f, us:%f, ue:%f, vs:%f, ve:%f", width, height, uStart, uEnd, vStart, vEnd);
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
        tmpVertices_ = new GLfloat[30];

        tmpVertices_[0] = p0x;
        tmpVertices_[1] = p0y;
        tmpVertices_[2] = 0.0f;
        tmpVertices_[3] = p0u;
        tmpVertices_[4] = p0v;

        tmpVertices_[5] = p1x;
        tmpVertices_[6] = p1y;
        tmpVertices_[7] = 0.0f;
        tmpVertices_[8] = p1u;
        tmpVertices_[9] = p1v;

        tmpVertices_[10] = p2x;
        tmpVertices_[11] = p2y;
        tmpVertices_[12] = 0.0f;
        tmpVertices_[13] = p2u;
        tmpVertices_[14] = p2v;

        // Second triangle
        tmpVertices_[15] = p0x;
        tmpVertices_[16] = p0y;
        tmpVertices_[17] = 0.0f;
        tmpVertices_[18] = p0u;
        tmpVertices_[19] = p0v;

        tmpVertices_[20] = p2x;
        tmpVertices_[21] = p2y;
        tmpVertices_[22] = 0.0f;
        tmpVertices_[23] = p2u;
        tmpVertices_[24] = p2v;

        tmpVertices_[25] = p3x;
        tmpVertices_[26] = p3y;
        tmpVertices_[27] = 0.0f;
        tmpVertices_[28] = p3u;
        tmpVertices_[29] = p3v;
    }

    MeshAssetPrivate::~MeshAssetPrivate()
    {
        if(vbo_ != 0)
        {
            glDeleteBuffers(1, &vbo_);
        }

        if(tmpVertices_ != NULL)
        {
            delete [] tmpVertices_;
            tmpVertices_ = NULL;
        }
    }

    const std::vector<MeshAssetPrivate::StrideComponent>& MeshAssetPrivate::strideComponents() const
    {
        return strideComponents_;
    }

    void MeshAssetPrivate::bind()
    {
        loadGPUData();
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    }

    unsigned int MeshAssetPrivate::vertexCount()
    {
       return vertexCount_;
    }

    void MeshAssetPrivate::loadGPUData()
    {
        LOCK
        if(tmpVertices_ == NULL)
        {
            return; // we have created the GPU data already
        }

        glGenBuffers(1, &vbo_);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glBufferData(GL_ARRAY_BUFFER, 30 * sizeof(float), tmpVertices_, GL_STATIC_DRAW); // 30: 2 triangles * 3 points * (3 position floats + 2 uv floats)
        vertexCount_ = 6; // 6: 2 triangles * 3 points each

        // Set stride format
        StrideComponent xyz(std::string("xyz"), 5, 0, 3, GL_FLOAT);
        StrideComponent uv(std::string("uv"), 5, 3, 2, GL_FLOAT);
        strideComponents_.push_back(xyz);
        strideComponents_.push_back(uv);

        delete [] tmpVertices_;
        tmpVertices_ = NULL;
    }
}

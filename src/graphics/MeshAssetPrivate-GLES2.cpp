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

#include "Common.hpp"
#include "MeshAssetPrivate.hpp"
#include "w/base/Log.hpp"
#include "w/base/Exception.hpp"

namespace w
{
    namespace graphics
    {
        MeshAssetPrivate::MeshAssetPrivate(const std::vector<StrideComponent> & strideComponents, char* tmpVertexData, unsigned int vertexCount):
            strideComponents_(strideComponents),
            tmpVertexData_((GLfloat*)tmpVertexData),
            vertexCount_(vertexCount)
        {
            if(strideComponents_.size() == 0)
            {
                throw Exception("MeshAsset strideComponents size should be > 0");
            }
            StrideComponent tmp = strideComponents.back();
            strideByteSize_ = tmp.byteOffset + tmp.numberOfComponents * sizeof(GLfloat);
        }

        MeshAssetPrivate::MeshAssetPrivate(float width, float height, float uStart, float uEnd, float vStart, float vEnd):
            width_(width),
            height_(height),
            vbo_(0),
            tmpVertexData_(NULL),
            vertexCount_(0)
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
            tmpVertexData_ = new GLfloat[30];

            tmpVertexData_[0] = p0x;
            tmpVertexData_[1] = p0y;
            tmpVertexData_[2] = 0.0f;
            tmpVertexData_[3] = p0u;
            tmpVertexData_[4] = p0v;

            tmpVertexData_[5] = p1x;
            tmpVertexData_[6] = p1y;
            tmpVertexData_[7] = 0.0f;
            tmpVertexData_[8] = p1u;
            tmpVertexData_[9] = p1v;

            tmpVertexData_[10] = p2x;
            tmpVertexData_[11] = p2y;
            tmpVertexData_[12] = 0.0f;
            tmpVertexData_[13] = p2u;
            tmpVertexData_[14] = p2v;

            // Second triangle
            tmpVertexData_[15] = p0x;
            tmpVertexData_[16] = p0y;
            tmpVertexData_[17] = 0.0f;
            tmpVertexData_[18] = p0u;
            tmpVertexData_[19] = p0v;

            tmpVertexData_[20] = p2x;
            tmpVertexData_[21] = p2y;
            tmpVertexData_[22] = 0.0f;
            tmpVertexData_[23] = p2u;
            tmpVertexData_[24] = p2v;

            tmpVertexData_[25] = p3x;
            tmpVertexData_[26] = p3y;
            tmpVertexData_[27] = 0.0f;
            tmpVertexData_[28] = p3u;
            tmpVertexData_[29] = p3v;

            // Set stride format
            StrideComponent xyz(std::string("xyz"), 0, 3, GL_FLOAT);
            StrideComponent uv(std::string("uv"), 3* sizeof(GLfloat), 2, GL_FLOAT);
            strideComponents_.push_back(xyz);
            strideComponents_.push_back(uv);
            strideByteSize_ = 5 * sizeof(GLfloat); // 20
            vertexCount_ = 6; // 6: 2 triangles * 3 points each
        }

        MeshAssetPrivate::~MeshAssetPrivate()
        {
            if(vbo_ != 0)
            {
                glDeleteBuffers(1, &vbo_);
            }

            if(tmpVertexData_ != NULL)
            {
                delete [] tmpVertexData_;
                tmpVertexData_ = NULL;
            }
        }

        const std::vector<StrideComponent>& MeshAssetPrivate::strideComponents() const
        {
            return strideComponents_;
        }

        unsigned int MeshAssetPrivate::strideByteSize() const
        {
            return strideByteSize_;
        }

        void MeshAssetPrivate::bind()
        {
            loadGPUData();
            glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        }

        unsigned int MeshAssetPrivate::vertexCount() const
        {
           return vertexCount_;
        }

        void MeshAssetPrivate::loadGPUData()
        {
            LOCK
            if(tmpVertexData_ == NULL)
            {
                return; // we have created the GPU data already
            }

            glGenBuffers(1, &vbo_);
            glBindBuffer(GL_ARRAY_BUFFER, vbo_);
            glBufferData(GL_ARRAY_BUFFER, vertexCount_ * strideByteSize_, tmpVertexData_, GL_STATIC_DRAW); // 30: 2 triangles * 3 points * (3 position floats + 2 uv floats)

            delete [] tmpVertexData_;
            tmpVertexData_ = NULL;
        }

        float MeshAssetPrivate::width() const
        {
            return width_;
        }

        float MeshAssetPrivate::height() const
        {
            return height_;
        }
    }
}

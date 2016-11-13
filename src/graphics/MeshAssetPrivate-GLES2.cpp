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
#include "w/base/Exception.hpp"
#ifdef __linux__ // & Android
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else // APPLE
#include <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#endif

namespace w
{
    namespace graphics
    {
        

        MeshAssetPrivate::MeshAssetPrivate(
                const std::vector<StrideComponent> & strideComponents,
                float* tmpVertexData,
                unsigned int vertexCount,
                Aabb const& aabb):
            id_(0),
#ifdef __APPLE__
            id2_(0),
#endif
            strideComponents_(strideComponents),
            tmpVertexData_((GLfloat*)tmpVertexData),
            vertexCount_(vertexCount),
            aabb_(aabb)
        {
            if(strideComponents_.size() == 0)
            {
                throw Exception("MeshAsset strideComponents size should be > 0");
            }
            StrideComponent tmp = strideComponents.back();
            strideByteSize_ = tmp.byteOffset + tmp.numberOfComponents * sizeof(GLfloat);
        }
        
        MeshAssetPrivate::MeshAssetPrivate(float width, float height,
                float uStart, float uEnd, float vStart, float vEnd,
                float wOffset, float hOffset):
            id_(0),
#ifdef __APPLE__
            id2_(0),
#endif
            strideComponents_(),
            tmpVertexData_(NULL),
            vertexCount_(0),
            aabb_(Eigen::Vector3f(wOffset, hOffset, 0.0f),
                    Eigen::Vector3f(wOffset + width, hOffset + height, 0.0f))
        {
            // fill
            tmpVertexData_ = new GLfloat[30];
            fillWithRectangleData(tmpVertexData_, wOffset, hOffset, width, height, uStart, uEnd, vStart, vEnd);

            // Set stride format
            StrideComponent xyz(std::string("xyz"), 0, 3, StrideType::Float32);
            StrideComponent uv(std::string("uv"), 3 * sizeof(GLfloat), 2, StrideType::Float32);
            strideComponents_.push_back(xyz);
            strideComponents_.push_back(uv);
            strideByteSize_ = 5 * sizeof(GLfloat); // 20
            vertexCount_ = 6; // 6: 2 triangles * 3 points each
        }

        MeshAssetPrivate::~MeshAssetPrivate()
        {
            if(id_ != 0)
            {
#ifdef __linux__ // & Android
                glDeleteBuffers(1, &id_);
#else // APPLE
                glDeleteVertexArraysOES(1, &id_);
                glDeleteBuffers(1, &id2_);
#endif
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

        void MeshAssetPrivate::bind(w::graphics::ShaderProgramAssetPrivate* program)
        {
            loadGPUData(program);
#ifdef __linux__ // & Android
            glBindBuffer(GL_ARRAY_BUFFER, id_);
#else // APPLE
            glBindVertexArrayOES(id_);
#endif
        }

        unsigned int MeshAssetPrivate::vertexCount() const
        {
           return vertexCount_;
        }

        void MeshAssetPrivate::setData(const std::vector<StrideComponent> & strideComponents, float* vertexData, unsigned int vertexCount, Aabb const& aabb)
        {
            if(tmpVertexData_ != NULL)
            {
                delete [] tmpVertexData_;
            }
            strideComponents_ = strideComponents;
            tmpVertexData_ = (GLfloat*)vertexData;
            vertexCount_ = vertexCount;
            StrideComponent tmp = strideComponents.back();
            strideByteSize_ = tmp.byteOffset + tmp.numberOfComponents * sizeof(GLfloat);
            aabb_ = aabb;
        }

        Aabb const& MeshAssetPrivate::aabb() const
        {
            return aabb_;
        }

        void MeshAssetPrivate::loadGPUData(w::graphics::ShaderProgramAssetPrivate* program)
        {
            if(tmpVertexData_ == NULL)
            {
                return; // we have created the GPU data already
            }

#ifdef __linux__ // & Android
            {
                if(id_ != 0)
                {
                    glDeleteBuffers(1, &id_);
                }
                glGenBuffers(1, &id_);
                glBindBuffer(GL_ARRAY_BUFFER, id_);
                glBufferData(GL_ARRAY_BUFFER, vertexCount_ * strideByteSize_, tmpVertexData_, GL_STATIC_DRAW);
            }
#else // APPLE
            {
                // Vertex Array Object
                glGenVertexArraysOES(1, &id_);
                glBindVertexArrayOES(id_);

                // Vertex data
                glGenBuffers(1, &id2_);
                glBindBuffer(GL_ARRAY_BUFFER, id2_);
                glBufferData(GL_ARRAY_BUFFER, vertexCount_ * strideByteSize_, tmpVertexData_, GL_STATIC_DRAW);
                
                // Attributes
                for (auto iter = strideComponents_.begin(); iter != strideComponents_.end(); iter++)
                {
                    GLint tmp = program->attribute((*iter).shaderSymbolName);
                    glEnableVertexAttribArray(tmp);
                    
                    //LOGD("index: %d, symbol id: %d", index, program->attribute((*iter).shaderSymbolName));
                    glVertexAttribPointer(
                                          tmp,
                                          (*iter).numberOfComponents,
                                          GL_FLOAT,
                                          GL_FALSE,
                                          strideByteSize_,
                                          (GLvoid*)((*iter).byteOffset));
                }

                // Vertex Array Object, unbind
                glBindVertexArrayOES(0);
            }
#endif
            
            delete [] tmpVertexData_;
            tmpVertexData_ = NULL;
        }

        void MeshAssetPrivate::fillWithRectangleData(GLfloat* vertexData, float x, float y, float w, float h, float uStart, float uEnd, float vStart, float vEnd)
        {
            /*
             * For rectangle, we create has two triangles:
             *
             * 1_____2
             *  |  /|
             *  | / |
             *  |/__|
             * 0     3
             *
             */

            // Bottom left corner
            float p0x = x;
            float p0y = y;
            float p0u = uStart;
            float p0v = vStart;

            // Top left corner
            float p1x = x;
            float p1y = h + y;
            float p1u = uStart;
            float p1v = vEnd;

            // Top right corner
            float p2x = w + x;
            float p2y = h + y;
            float p2u = uEnd;
            float p2v = vEnd;

            // Bottom right corner
            float p3x = w + x;
            float p3y = y;
            float p3u = uEnd;
            float p3v = vStart;

            // Fill vertex data
            vertexData[0] = p0x;
            vertexData[1] = p0y;
            vertexData[2] = 0.0f;
            vertexData[3] = p0u;
            vertexData[4] = p0v;

            vertexData[5] = p1x;
            vertexData[6] = p1y;
            vertexData[7] = 0.0f;
            vertexData[8] = p1u;
            vertexData[9] = p1v;

            vertexData[10] = p2x;
            vertexData[11] = p2y;
            vertexData[12] = 0.0f;
            vertexData[13] = p2u;
            vertexData[14] = p2v;

            // Second triangle
            vertexData[15] = p0x;
            vertexData[16] = p0y;
            vertexData[17] = 0.0f;
            vertexData[18] = p0u;
            vertexData[19] = p0v;

            vertexData[20] = p2x;
            vertexData[21] = p2y;
            vertexData[22] = 0.0f;
            vertexData[23] = p2u;
            vertexData[24] = p2v;

            vertexData[25] = p3x;
            vertexData[26] = p3y;
            vertexData[27] = 0.0f;
            vertexData[28] = p3u;
            vertexData[29] = p3v;
        }
    }
}

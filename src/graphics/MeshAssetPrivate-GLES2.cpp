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
            strideComponents_(),
            tmpVertexData_(NULL),
            vertexCount_(0),
            aabb_(Eigen::Vector3f(wOffset, hOffset, 0.0f),
                    Eigen::Vector3f(wOffset + width, hOffset + height, 0.0f))
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
            float p0x = 0.0f + wOffset;
            float p0y = 0.0f + hOffset;
            float p0u = uStart;
            float p0v = vStart;

            // Top left corner
            float p1x = 0.0f + wOffset;
            float p1y = height + hOffset;
            float p1u = uStart;
            float p1v = vEnd;

            // Top right corner
            float p2x = width + wOffset;
            float p2y = height + hOffset;
            float p2u = uEnd;
            float p2v = vEnd;

            // Bottom right corner
            float p3x = width + wOffset;
            float p3y = 0.0f + hOffset;
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
                // TODO
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
                GLuint vBuffer;
                glGenBuffers(1, &vBuffer);
                glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
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
    }
}

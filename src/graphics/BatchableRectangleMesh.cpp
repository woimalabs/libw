/**
 * libw
 *
 * Copyright (C) 2013-2016 Woima Solutions
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

#include "w/graphics/BatchableRectangleMesh.hpp"
#include <w/base/FileHandle.hpp>
#include <w/base/ResourceManager.hpp>
#include <w/base/String.hpp>
#include "MeshAssetPrivate.hpp"

namespace w
{
    namespace graphics
    {
        BatchableRectangleMesh::BatchableRectangleMesh(float x, float y, float w, float h, float uStart, float uEnd, float vStart, float vEnd):
            Referenced(),
            x_(x),
            y_(y),
            w_(w),
            h_(h),
            uStart_(uStart),
            uEnd_(uEnd),
            vStart_(vStart),
            vEnd_(vEnd)
        {
        }

        BatchableRectangleMesh::~BatchableRectangleMesh()
        {
        }

        w::graphics::MeshAsset BatchableRectangleMesh::create(std::vector<ReferencedPointer<BatchableRectangleMesh> > const& list, Aabb const& aabb)
        {
            // Stride format
            std::vector<StrideComponent> strideComponents;
            StrideComponent xyz(std::string("xyz"), 0, 3, StrideType::Float32);
            StrideComponent uv(std::string("uv"), 3 * sizeof(GLfloat), 2, StrideType::Float32);
            strideComponents.push_back(xyz);
            strideComponents.push_back(uv);
            //strideByteSize_ = 5 * sizeof(GLfloat); // (xyzuv => 5)floats * 4 byte/float = 20 bytes
            unsigned int vertexCount = list.size() * 6; // 6: 2 triangles * 3 points each;

            // Vertex data
            float* vertexData = new GLfloat[vertexCount * 5];
            
            // Fill data
            unsigned int vI = 0;
            for(auto i = list.begin();
                i != list.end();
                i++)
            {
                auto j = (*i).pointer();
                MeshAssetPrivate::fillWithRectangleData(&vertexData[vI], j->x_, j->y_, j->w_, j->h_, j->uStart_, j->uEnd_, j->vStart_, j->vEnd_);
                vI += 30;
            }

            return MeshAsset(strideComponents, vertexData, vertexCount, aabb);
        }

        w::graphics::BatchableRectangleMesh* BatchableRectangleMesh::copy()
        {
            return new BatchableRectangleMesh(
                x_, y_, w_, h_,
                uStart_, uEnd_, vStart_, vEnd_);
        }

        void BatchableRectangleMesh::move(float x)
        {
            x_ += x;
        }

        float BatchableRectangleMesh::width() const
        {
            return w_;
        }

        /*std::map<std::string, ReferencedPointer<w::graphics::BatchableRectangleMesh> > BatchableRectangleMesh::loadAtlas(std::string const& mappingFile, w::graphics::TextureAsset const& texture)
        {
            std::map<std::string, ReferencedPointer<w::graphics::BatchableRectangleMesh> > r;

            float w = texture.width() * ResourceManager::graphicsDownScale();
            float h = texture.height() * ResourceManager::graphicsDownScale();

            // load atlas file
            ReferencedPointer<FileHandle> file = ResourceManager::bundledFile(mappingFile);
            unsigned int size = file.pointer()->byteSize();
            char* buffer = new char[size + 1];
            file.pointer()->read(buffer, size);
            buffer[size] = 0;
            std::string tmp(buffer); // TODO: remove this copy
            delete [] buffer;
            std::vector<std::string> list = w::String::split(tmp, '\n');

            // atlas items
            for(std::vector<std::string>::iterator i = list.begin(); i != list.end(); i++)
            {
                std::vector<std::string> tmp = w::String::split(*i, ':');
                std::string key = tmp[0];
                std::vector<std::string> tmp2 = w::String::split(tmp[1], ' ');
                std::vector<std::string> location = w::String::split(tmp2[0], ',');
                std::vector<std::string> size = w::String::split(tmp2[1], ',');
                unsigned int locationX = w::String::toInt(location[0]);
                unsigned int locationY = w::String::toInt(location[1]);
                unsigned int sizeX = w::String::toInt(size[0]);
                unsigned int sizeY = w::String::toInt(size[1]);

                // create batchable mesh
                {
                    std::pair<std::string, ReferencedPointer<w::graphics::BatchableRectangleMesh> > batchable = std::make_pair(key, 
                        ReferencedPointer<w::graphics::BatchableRectangleMesh>(
                            new w::graphics::BatchableRectangleMesh(
                                0.0f,
                                0.0f,
                                (float)sizeX,
                                (float)sizeY,
                                (float)(locationX) / w,
                                (float)(locationX + sizeX) / w,
                                (float)(locationY) / h,
                                (float)(locationY + sizeY) / h)
                            )
                        );
                    r.insert(batchable);
                }
            }

            return r;
        }*/

        BatchableRectangleMeshAtlas::BatchableRectangleMeshAtlas(std::string const& mappingFile, w::graphics::TextureAsset const& atlasTexture)
        {
            float w = atlasTexture.width() * ResourceManager::graphicsDownScale();
            float h = atlasTexture.height() * ResourceManager::graphicsDownScale();

            // load atlas file
            ReferencedPointer<FileHandle> file = ResourceManager::bundledFile(mappingFile);
            unsigned int size = file.pointer()->byteSize();
            char* buffer = new char[size + 1];
            file.pointer()->read(buffer, size);
            buffer[size] = 0;
            std::string tmp(buffer); // TODO: remove this copy
            delete [] buffer;
            std::vector<std::string> list = w::String::split(tmp, '\n');

            // atlas items
            for(std::vector<std::string>::iterator i = list.begin(); i != list.end(); i++)
            {
                std::vector<std::string> tmp = w::String::split(*i, ':');
                std::string key = tmp[0];
                std::vector<std::string> tmp2 = w::String::split(tmp[1], ' ');
                std::vector<std::string> location = w::String::split(tmp2[0], ',');
                std::vector<std::string> size = w::String::split(tmp2[1], ',');
                unsigned int locationX = w::String::toInt(location[0]);
                unsigned int locationY = w::String::toInt(location[1]);
                unsigned int sizeX = w::String::toInt(size[0]);
                unsigned int sizeY = w::String::toInt(size[1]);

                // create batchable mesh
                {
                    std::pair<std::string, ReferencedPointer<w::graphics::BatchableRectangleMesh> > batchable = std::make_pair(key, 
                        ReferencedPointer<w::graphics::BatchableRectangleMesh>(
                            new w::graphics::BatchableRectangleMesh(
                                0.0f,
                                0.0f,
                                (float)sizeX,
                                (float)sizeY,
                                (float)(locationX) / w,
                                (float)(locationX + sizeX) / w,
                                (float)(locationY) / h,
                                (float)(locationY + sizeY) / h)
                            )
                        );
                    //LOGD("adding to atlas: %s", key.c_str());
                    atlas_.insert(batchable);
                }
            }
        }

        BatchableRectangleMeshAtlas::~BatchableRectangleMeshAtlas()
        {
        }

        ReferencedPointer<w::graphics::BatchableRectangleMesh> BatchableRectangleMeshAtlas::batchable(std::string const& key)
        {
            auto i = atlas_.find(key);
            if(i != atlas_.end())
            {
                //LOGD("found key: %s", key.c_str());
                return ReferencedPointer<w::graphics::BatchableRectangleMesh>(i->second.pointer()->copy());
            }

            LOGE("batchable key not found: %s", key.c_str());
            return ReferencedPointer<w::graphics::BatchableRectangleMesh>(NULL);
        }
    }
}

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

#include "PolygonAssetPrivate.hpp"
#include "w/base/Log.hpp"

namespace w
{
    PolygonAssetPrivate::PolygonAssetPrivate(const std::vector<std::vector<PolygonAsset::Point> > & data):
        vbo_(0),
        tmpData_(new std::vector<std::vector<PolygonAsset::Point> >(data))
    {
    }

    PolygonAssetPrivate::~PolygonAssetPrivate()
    {
        if(vbo_ != 0)
        {
            glDeleteBuffers(1, &vbo_);
        }

        if(tmpData_ != NULL)
        {
            delete [] tmpData_;
            tmpData_ = NULL;
        }
    }

    void PolygonAssetPrivate::bind()
    {
        loadGPUData();
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    }

    void PolygonAssetPrivate::loadGPUData()
    {
        LOCK

        if(tmpData_ == NULL)
        {
            return; // we have created the GPU data already
        }
/*
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
        tmpVertices_ = NULL;*/
    }
}

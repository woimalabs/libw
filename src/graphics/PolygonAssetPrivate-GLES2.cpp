/**
 * libw
 *
 * Copyright (C) 2013-2014 Woima Solutions
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
    namespace graphics
    {
        PolygonAssetPrivate::PolygonAssetPrivate(const std::vector<PolygonAsset::Point> & data):
            vbo_(0),
            tmpData_(new std::vector<PolygonAsset::Point>(data)),
            pointCount_(0)
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

        unsigned int PolygonAssetPrivate::pointCount() const
        {
           return pointCount_;
        }

        const std::vector<StrideComponent>& PolygonAssetPrivate::strideComponents() const
        {
            return strideComponents_;
        }

        void PolygonAssetPrivate::bind()
        {
           // loadGPUData();
           // glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        }

        void PolygonAssetPrivate::loadGPUData()
        {
            LOCK

            /*if(tmpData_ == NULL)
            {
                return; // we have created the GPU data already
            }

            // fake data
            float fakeData[12] =
            {
                0.0f, 0.0f, 10.0f,
                10.0f, 2.0f, 10.0f,
                10.0f, 4.0f, 10.0f,
                0.0f, 6.0f, 10.0f
            };


            glGenBuffers(1, &vbo_);
            glBindBuffer(GL_ARRAY_BUFFER, vbo_);
            glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), (const GLvoid*)&(tmpData_->front())  address of first in C++ , GL_STATIC_DRAW);
            pointCount_ = 2;
            // glBufferData(GL_ARRAY_BUFFER, tmpData_->size() * 3 * sizeof(float), (const GLvoid*)&(tmpData_->front())  address of first in C++  GL_STATIC_DRAW); // point count * 3 position floats
            //pointCount_ = tmpData_->size();

            // Set stride format
            StrideComponent xyz(std::string("xyz"), 3, 0, 3, GL_FLOAT);
            strideComponents_.push_back(xyz);

            delete tmpData_;
            tmpData_ = NULL;*/
        }
    }
}

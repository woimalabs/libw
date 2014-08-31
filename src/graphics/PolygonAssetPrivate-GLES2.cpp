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
        PolygonAssetPrivate::PolygonAssetPrivate(const std::vector<PolygonAsset::Point> & data,
            const Eigen::Vector4f & color):
            color_(color),
            tmpData_(new std::vector<PolygonAsset::Point>(data)),
            vbo_(0),
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
                delete tmpData_;
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

        Eigen::Vector4f PolygonAssetPrivate::color()
        {
            return color_;
        }

        void PolygonAssetPrivate::bind()
        {
            loadGPUData();
            glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        }

        void PolygonAssetPrivate::loadGPUData()
        {
            if(tmpData_ == NULL)
            {
                return; // we have created the GPU data already
            }
            LOCK

            /* Debug
            std::vector<PolygonAsset::Point>::const_iterator last = tmpData_->begin();
            std::vector<PolygonAsset::Point>::const_iterator i = last + 1;
            for(; last != tmpData_->end(); last = i, i++)
            {
                PolygonAsset::Point j0 = *last;
                PolygonAsset::Point j1 = *i;
                LOGD("PolygonAssetPrivateoutline:(%f, %f, %f), (%f, %f, %f)", j0.x, j0.y, j0.z, j1.x, j1.y, j1.z)
            }*/

            GLfloat* tmpData = (GLfloat*)&(tmpData_->front());
            unsigned int tmpSize = tmpData_->size() * sizeof(PolygonAsset::Point);
            glGenBuffers(1, &vbo_);
            glBindBuffer(GL_ARRAY_BUFFER, vbo_);
            glBufferData(GL_ARRAY_BUFFER, tmpSize, tmpData, GL_STATIC_DRAW);
            pointCount_ = tmpData_->size();

            delete tmpData_;
            tmpData_ = NULL;
        }
    }
}

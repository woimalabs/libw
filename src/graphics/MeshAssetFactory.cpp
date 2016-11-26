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

#include "w/graphics/MeshAssetFactory.hpp"
#include "w/graphics/Stride.hpp"
#include "w/graphics/Aabb.hpp"

namespace w
{
    namespace graphics
    {
        MeshAssetFactory::MeshAssetFactory()
        {
        }

        MeshAssetFactory::~MeshAssetFactory()
        {
        }

        MeshAsset MeshAssetFactory::createGridWithRepeatedTexture(float width, float height, unsigned int rows, unsigned int cols)
        {
            std::vector<StrideComponent> strideComponents;
            StrideComponent xyz(std::string("xyz"), 0, 3, StrideType::Float32);
            StrideComponent uv(std::string("uv"), 3 * sizeof(GLfloat), 2, StrideType::Float32);
            strideComponents.push_back(xyz);
            strideComponents.push_back(uv);

            unsigned int vertexCount = rows * cols * 6;
            Aabb aabb = Aabb(Eigen::Vector3f(0.0f, 0.0f, 0.0f), Eigen::Vector3f(width, height, 0.0f));
            float* vertexData = new float[vertexCount * 5];

            // fill grid
            float xStep = width / (float)cols;
            float yStep = height / (float)rows;
            float xCurrent = 0.0f; 
            unsigned int i = 0;
            for(unsigned int x = 0; x < cols; x++)
            {
                float yCurrent = 0.0f; 
                for(unsigned int y = 0; y < rows; y++)
                {
                    fillWithRectangleData(&vertexData[i], xCurrent, yCurrent, xStep, yStep, 0.0f, 1.0f, 0.0f, 1.0f);
                    i += 30;
                    yCurrent += yStep;
                }
                xCurrent += xStep;
            }

            return MeshAsset(strideComponents, vertexData, vertexCount, aabb);
        }

        void MeshAssetFactory::fillWithRectangleData(GLfloat* vertexData, float x, float y, float w, float h, float uStart, float uEnd, float vStart, float vEnd)
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

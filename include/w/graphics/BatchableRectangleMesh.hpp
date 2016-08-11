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

#ifndef LIBW_GRAPHICS_BATCHABLERECTANGLEMESH
#define LIBW_GRAPHICS_BATCHABLERECTANGLEMESH

#include <w/base/Referenced.hpp>
#include <w/base/ReferencedPointer.hpp>
#include <w/base/Class.hpp>
#include <w/graphics/BatchableRectangleMesh.hpp>
#include <w/graphics/TextureAsset.hpp>
#include <w/graphics/MeshAsset.hpp>
#include <w/graphics/Aabb.hpp>
#include <vector>
#include <map>
#include <string>

namespace w
{
    namespace graphics
    {
        class BatchableRectangleMeshAtlas;

        class BatchableRectangleMesh: public Referenced
        {
        public:
            UNCOPYABLE(BatchableRectangleMesh)

            /**
             * Creates a rectangular mesh for batching from given data.
             *
             * @param [in]  x       X is the x location of mesh.
             * @param [in]  y       Y is the y location of mesh.
             * @param [in]  w       Width of the created mesh
             * @param [in]  h       Height of the created mesh
             * @param [in]  uStart  Texture u coodinate left
             * @param [in]  uEnd    Texture u coodinate right
             * @param [in]  vStart  Texture v coodinate bottom
             * @param [in]  vEnd    Texture u coodinate ceiling
             */
            BatchableRectangleMesh(float x, float y, float w, float h, float uStart, float uEnd, float vStart, float vEnd);
            ~BatchableRectangleMesh();
            static w::graphics::MeshAsset create(std::vector<ReferencedPointer<BatchableRectangleMesh> > const&, Aabb const&);
            w::graphics::BatchableRectangleMesh* copy(); // creates a copy of the instance
            void move(float x);
            float width() const;

        private:
            friend class BatchableRectangleMeshAtlas;
            float x_;
            float y_;
            float w_;
            float h_;
            float uStart_;
            float uEnd_;
            float vStart_;
            float vEnd_;
        };

        class BatchableRectangleMeshAtlas
        {
        public:
            UNCOPYABLE(BatchableRectangleMeshAtlas)

            BatchableRectangleMeshAtlas(std::string const& mappingFile, w::graphics::TextureAsset const& atlasTexture);
            ~BatchableRectangleMeshAtlas();

            /**
             * @param [in]  key to look for.
             *
             * returns a new instance of batchable if key found. Otherwise returns NULL.
             */
            ReferencedPointer<w::graphics::BatchableRectangleMesh> batchable(std::string const&);

        private:
            std::map<std::string, ReferencedPointer<w::graphics::BatchableRectangleMesh> > atlas_;
        };
    }
}

#endif

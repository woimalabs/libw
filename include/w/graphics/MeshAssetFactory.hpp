/**
 * libw
 *
 * Copyright (C) 2013-2017 Woima Solutions
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

#ifndef LIBW_GRAPHICS_MESHASSETFACTORY
#define LIBW_GRAPHICS_MESHASSETFACTORY

#include <w/base/Class.hpp>
#include <w/graphics/MeshAsset.hpp>
#ifdef __linux__ // & Android
    #include <GLES2/gl2.h>
#else // APPLE
    #include <OpenGLES/ES2/gl.h>
#endif

namespace w
{
    namespace graphics
    {
        class MeshAssetFactory
        {
        public:
            UNCOPYABLE(MeshAssetFactory)

            /**
             * Creates a grid mesh.
             *
             * @param [in] width of the mesh.
             * @param [in] height of the mesh.
             * @param [in] rows in the mesh.
             * @param [in] cols in the mesh.
             */
            static MeshAsset createGridWithRepeatedTexture(float width, float height, unsigned int rows, unsigned int cols);

            /**
             * Creates a 3x3 grid mesh for resizable textures with caps similar to "nine patch".
             *
             * @note assumes that the TextureAsset used with this MeshAsset will be a square.
             * @note assumes that the TextureAsset is power-of-two sized.
             *
             * @param [in] width of the mesh.
             * @param [in] height of the mesh.
             * @param [in] worldSideSize is the sides world size. Note that the center rectangle size is now -2*side size from width and height.
             * @param [in] textureUVSideSize size of the side in overall texture.
             */
            static MeshAsset createResizableTextureMeshWithCaps(float width, float height, float side, float textureSideSize);

        private:
            friend class BatchableRectangleMesh;
            friend class MeshAssetPrivate;
            static void fillWithRectangleData(GLfloat* vertexData, float x, float y, float w, float h, float uStart, float uEnd, float vStart, float vEnd);
            MeshAssetFactory();
            ~MeshAssetFactory();
        };
    }
}

#endif

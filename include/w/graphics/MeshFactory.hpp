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

/* XXX: dead code!

#ifndef LIBW_GRAPHICS_MESHASSETFACTORY
#define LIBW_GRAPHICS_MESHASSETFACTORY

#include <w/base/Class.hpp>
#include <vector>


namespace w
{
    namespace graphics
    {
        class Aabb;

        class MeshAssetFactory
        {
        public:
            UNCOPYABLE(MeshAssetFactory)

            **
             * Creates a grid mesh.
             *
             * @param [in]  width of the mesh.
             * @param [in]  height of the mesh.
             *
            static MeshAsset createGridWithRepeatedTextureUV(float width, float height, float grid);

            **
             * Creates a 3x3 grid mesh.
             *
             * @param [in]  width of the mesh.
             * @param [in]  height of the mesh.
             * @param [in]  side size. Note that the center rectangle size is now -2*side size from width and height.
             *
            static MeshAsset createRectangleWithCaps(float width, float height, float side);
        private:
            MeshFactory;
            ~MeshFactory;
        };
    }
}

#endif

*/

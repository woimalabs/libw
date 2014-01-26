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

#ifndef LIBW_RENDERER
#define LIBW_RENDERER

#include <w/graphics/TextureAsset.hpp>
#include <w/graphics/MeshAsset.hpp>
#include <w/graphics/ShaderProgramAsset.hpp>
#include <w/graphics/PolygonAsset.hpp>
#include <w/base/Class.hpp>

namespace w
{
    namespace graphics
    {
        class Renderer
        {
        public:
            COPYABLE(Renderer)

            Renderer();
            ~Renderer();
            void setBlend(bool value);
            void setDepthTest(bool value);
            void setCullFace(bool value);
            void draw(PolygonAsset const&, ShaderProgramAsset const&);
            void draw(TextureAsset const&, MeshAsset const&, ShaderProgramAsset const&);

            // test
            void drawLine(float p0x, float p0y, float p1x, float p1y, ShaderProgramAsset const& shaderProgram);

        private:
            class RendererPrivate* private_;
        };
    }
}

#endif

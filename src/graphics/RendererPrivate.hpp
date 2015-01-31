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

#ifndef LIBW_GRAPHICS_RENDERERPRIVATE
#define LIBW_GRAPHICS_RENDERERPRIVATE

#include <w/base/Class.hpp>
#include <w/base/Referenced.hpp>
#include <w/graphics/Renderer.hpp>
#include <w/graphics/TextureAsset.hpp>
#include <w/graphics/MeshAsset.hpp>
#include <w/graphics/PolygonAsset.hpp>
#include <w/graphics/FrameBuffer.hpp>
#include <w/graphics/ShaderProgramAsset.hpp>
#ifdef __linux__ // & Android
    #include <GLES2/gl2.h>
#else // APPLE
    #include <OpenGLES/ES2/gl.h>
#endif

namespace w
{
    namespace graphics
    {
        class RendererPrivate: public Referenced
        {
        public:
            UNCOPYABLE(RendererPrivate)

            RendererPrivate();
            ~RendererPrivate();
            void setBlend(bool value);
            void setBlendFunction(Renderer::BlendMode::Enum source, Renderer::BlendMode::Enum destination);
            void setBlendFunctionSeparate(Renderer::BlendMode::Enum sourceRGB,
                Renderer::BlendMode::Enum destinationRGB,
                Renderer::BlendMode::Enum sourceAlpha,
                Renderer::BlendMode::Enum destinationAlpha);
            void setDepthTest(bool value);
            void setCullFace(bool value);
            void setColorMask(bool r, bool g, bool b, bool a);

            /**
             * ShaderProgramAsset needs to have next attributes:
             *  vertex shader:
             *  -"attribute vec3 xyz;"
             *  -"attribute vec2 uv;"
             */
            void draw(const TextureAsset &, const MeshAsset &, const ShaderProgramAsset &);

            /**
             * ShaderProgramAsset needs to have next attributes:
             *  vertex shader:
             *  -"attribute vec3 xyz;"
             */
            void draw(const PolygonAsset & polygon, const ShaderProgramAsset & shaderProgram);

            /**
             */
            void draw(const FrameBuffer &, const MeshAsset &, const ShaderProgramAsset &);

            /**
             * ShaderProgramAsset needs to have "attribute vec3 xyz;"
             */
            void drawLine(float p0x, float p0y, float p1x, float p1y, const ShaderProgramAsset & shaderProgram);


        private:

        };
    }
}

#endif

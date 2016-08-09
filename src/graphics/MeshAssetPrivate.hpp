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

#ifndef LIBW_GRAPHICS_MESHASSETPRIVATE
#define LIBW_GRAPHICS_MESHASSETPRIVATE

#include "w/graphics/Aabb.hpp"
#include "w/graphics/Stride.hpp"
#include "ShaderProgramAssetPrivate.hpp"
#include <w/base/Referenced.hpp>
#include <w/base/Class.hpp>
#ifdef __linux__ // & Android
    #include <GLES2/gl2.h>
#else // APPLE
    #include <OpenGLES/ES2/gl.h>
#endif
#include <string>
#include <vector>

namespace w
{
    namespace graphics
    {
        class MeshAssetPrivate: public Referenced
        {
        public:
            UNCOPYABLE(MeshAssetPrivate)

            MeshAssetPrivate(const std::vector<StrideComponent> & strideComponents, float* tmpVertexData, unsigned int vertexCount, const Aabb& aabb);
            MeshAssetPrivate(float width, float height, float uStart, float uEnd, float vStart, float vEnd, float wOffset, float hOffset);

            virtual ~MeshAssetPrivate();

            const std::vector<StrideComponent>& strideComponents() const;
            unsigned int strideByteSize() const;
            void bind(w::graphics::ShaderProgramAssetPrivate* = NULL);
            unsigned int vertexCount() const;
            void setData(const std::vector<StrideComponent> & strideComponents, float* vertexData, unsigned int vertexCount, Aabb const& aabb);

            Aabb const& aabb() const;
            bool vaoBased_;

        private:
            void loadGPUData(w::graphics::ShaderProgramAssetPrivate* program);
            GLuint vbo_;
            GLuint vao_;
            std::vector<StrideComponent> strideComponents_;
            GLfloat* tmpVertexData_;
            unsigned int vertexCount_;
            unsigned int strideByteSize_;
            Aabb aabb_;
        };
    }
}

#endif

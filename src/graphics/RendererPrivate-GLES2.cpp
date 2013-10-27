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

#include "w/graphics/MeshAsset.hpp"
#include "RendererPrivate.hpp"
#include "TextureAssetPrivate.hpp"
#include "MeshAssetPrivate.hpp"
#include "ShaderProgramAssetPrivate.hpp"
#include <vector>
#include <string>

namespace w
{
    RendererPrivate::RendererPrivate():
        Referenced()
    {
    }

    RendererPrivate::~RendererPrivate()
    {
    }

    void RendererPrivate::draw(TextureAsset const& texture, MeshAsset const& mesh, ShaderProgramAsset const& shaderProgram)
    {
        // Use given texture
        texture.private_->bind();

        // Use given mesh
        mesh.private_->bind();

        // Find out needed data for shader
        const std::vector<MeshAssetPrivate::StrideComponent>& uniforms = mesh.private_->strideComponents();

        // Bind shader symbols to mesh data
        std::vector<std::string> shaderSymbols;
        for (std::vector<MeshAssetPrivate::StrideComponent>::const_iterator i = uniforms.begin(); i != uniforms.end(); i++)
        {
            GLint shaderSymbolLocation = shaderProgram.private_->uniform((*i).shaderSymbolName);
            glEnableVertexAttribArray(shaderSymbolLocation);
            //glVertexAttribPointer(shaderSymbolLocation, (*i).numberOfComponents. (*i).type, GL_TRUE, (*i).strideLength, (*i).strideOffset);
        }

        // Use given shaderProgram
        shaderProgram.private_->start();

        // Draw
        glDrawArrays(GL_TRIANGLES, 0, mesh.private_->vertexCount());
    }
}

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

#include "w/graphics/Renderer.hpp"
#include "RendererPrivate.hpp"

namespace w
{
    Renderer::Renderer():
        private_(new RendererPrivate())
    {
        private_->increment();
    }

    Renderer::Renderer(Renderer const& r):
        private_(r.private_)
    {
        private_->increment();
    }

    Renderer::~Renderer()
    {
        private_->decrement();
    }

    Renderer& Renderer::operator=(Renderer const& r)
    {
        if (this != &r)
        {
            private_ = r.private_;
            private_->increment();
        }
        return *this;
    }

    void Renderer::draw(TextureAsset const& texture, MeshAsset const& mesh, ShaderProgramAsset const& shaderProgram)
    {
        private_->draw(texture, mesh, shaderProgram);
    }

    void Renderer::drawLine(float p0x, float p0y, float p1x, float p1y, ShaderProgramAsset const& shaderProgram)
    {
        private_->drawLine(p0x, p0y, p1x, p1y, shaderProgram);
    }
}

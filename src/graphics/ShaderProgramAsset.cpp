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

#include "w/graphics/ShaderProgramAsset.hpp"
#include "ShaderProgramAssetPrivate.hpp"
#include "w/base/ResourceManagerPrivate.hpp"
#include <w/math/Eigen.hpp>

namespace w
{
    namespace graphics
    {
        ShaderProgramAsset::ShaderProgramAsset(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename):
            private_(NULL)
        {
            std::string id = "ShaderProgram:" + vertexShaderFilename + std::string(",") + fragmentShaderFilename;
            private_ = dynamic_cast<ShaderProgramAssetPrivate*>(ResourceManagerPrivate::assetPrivate(id));
            if (private_ == NULL)
            {
                private_ = new ShaderProgramAssetPrivate(vertexShaderFilename, fragmentShaderFilename);
                ResourceManagerPrivate::setAssetPrivate(id, private_);
            }

            private_->increment();
        }

        ShaderProgramAsset::ShaderProgramAsset(ShaderProgramAsset const& r):
            private_(r.private_)
        {
            private_->increment();
        }

        ShaderProgramAsset::~ShaderProgramAsset()
        {
            private_->decrement();
        }

        ShaderProgramAsset& ShaderProgramAsset::operator=(ShaderProgramAsset const& r)
        {
            if (this != &r)
            {
                private_ = r.private_;
                private_->increment();
            }
            return *this;
        }

        void ShaderProgramAsset::setUniform(const std::string& symbolName, float value)
        {
            private_->setUniform(symbolName, value);
        }

        void ShaderProgramAsset::setUniform(const std::string& symbolName, const Eigen::Matrix4f& value)
        {
            private_->setUniform(symbolName, value);
        }
    }
}

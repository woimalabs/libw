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

#include "w/scene/Component.hpp"
#include "w/base/Exception.hpp"

namespace w
{
    namespace scene
    {
        Component ComponentNull(NULL);

        Component::Component(ComponentPrivate* priv):
            private_(ReferencedPointer<ComponentPrivate>(priv))
        {
        }

        Component::Component(Component const& r):
            private_(r.private_)
        {
        }

        Component::~Component()
        {
        }

        Component& Component::operator=(Component const& r)
        {
            if (this != &r)
            {
                private_ = r.private_;
            }
            return *this;
        }

        bool Component::isNull() const
        {
            return private_.isNull();
        }

        unsigned int Component::id() const
        {
            return private_.pointer()->id();
        }

        const std::type_info& Component::typeId() const
        {
            return private_.pointer()->typeId();
        }

        ComponentPrivate* Component::pointer()
        {
            return private_.pointer();
        }
    }
}

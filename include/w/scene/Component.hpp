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

#ifndef LIBW_SCENE_COMPONENT
#define LIBW_SCENE_COMPONENT

#include "w/scene/ComponentPrivate.hpp"
#include "w/base/ReferencedPointer.hpp"
#include <w/base/Class.hpp>
#include <typeinfo>

namespace w
{
    namespace scene
    {
        class NodePrivate;

        #define REGISTER_COMPONENT(ComponentType) \
            static const std::string& typeId(void) \
            { \
                static const std::string r = ComponentType; \
                return r; \
            }

        /**
         * @class Component
         *
         * Inherit Component and ComponentPrivateTemplate to
         * create new component to scene.
         */
        class Component
        {
        public:
            COPYABLE(Component)

            Component(ComponentPrivate* private_);
            virtual ~Component();
            bool isNull() const;
            unsigned int id() const;
            const std::type_info& typeId() const;

        protected:
            friend class NodePrivate;
            ReferencedPointer<ComponentPrivate> private_;
        };

        extern Component ComponentNull;
    }
}

#endif

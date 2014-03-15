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

#ifndef LIBW_SCENE_NODEPRIVATE
#define LIBW_SCENE_NODEPRIVATE

#include "w/scene/Visitor.hpp"
#include "w/scene/Component.hpp"
#include <w/base/Class.hpp>
#include "w/base/Mutex.hpp"
#include "w/base/ReferencedPointer.hpp"
#include "w/base/Referenced.hpp"
#include <map>
#include <list>
#include <vector>

namespace w
{
    namespace scene
    {
        class Node;

        class NodePrivate: public Referenced
        {
        public:
            UNCOPYABLE(NodePrivate)

            NodePrivate();
            NodePrivate(Component const& c0);
            NodePrivate(Component const& c0, Component const& c1);
            NodePrivate(Component const& c0, Component const& c1, Component const& c2);
            NodePrivate(Component const& c0, Component const& c1, Component const& c2, Component const& c3);
            virtual ~NodePrivate();
            void removeComponent(Component const& component);
            void addComponent(Component const& component);
            ReferencedPointer<ComponentPrivate> component(std::string const& type);
            template<class T> ReferencedPointer<T> component();
            void addChild(NodePrivate* node);
            std::vector<ReferencedPointer<NodePrivate> > children();
            ReferencedPointer<NodePrivate> parent();

        private:
            Mutex mutexStructure_;
            std::list<NodePrivate*> children_;
            NodePrivate* parent_;

            Mutex mutexComponents_;
            std::map<std::string, ReferencedPointer<ComponentPrivate> > components_;
        };
    }
}

#endif

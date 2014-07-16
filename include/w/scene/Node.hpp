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

#ifndef LIBW_SCENE_NODE
#define LIBW_SCENE_NODE

#include "w/scene/NodePrivate.hpp"
#include "w/scene/Visitor.hpp"
#include "w/scene/Component.hpp"
#include <w/base/Class.hpp>
#include "w/base/Mutex.hpp"
#include "w/base/ReferencedPointer.hpp"
#include "w/base/Referenced.hpp"
#include "w/base/Mutex.hpp"
#include <vector>

namespace w
{
    namespace scene
    {
        /**
         * @class Node
         *
         * Class that creates tree structure to your scene. Node is unique
         * in the tree.
         *
         * TODO: If node is replaced in the tree the old placement
         * disappears _before_ the new placement is done.
         */
        class Node
        {
        public:
            COPYABLE(Node)

            Node();
            Node(Component const& c0);
            Node(Component const& c0, Component const& c1);
            Node(Component const& c0, Component const& c1, Component const& c2);
            Node(Component const& c0, Component const& c1, Component const& c2, Component const& c3);
            virtual ~Node();
            void accept(Visitor& visitor);
            void addChild(Node const& node);
            void removeChildWithComponentId(bool recursive, const std::vector<unsigned int> & ids);
            void removeChildren();
            void addComponent(Component const& component);

            template<class T> void removeComponent()
            {
                private_.pointer()->removeComponent<T>();
            }

            template<class T> T* component()
            {
                return private_.pointer()->component<T>();
            }

            template<class T> bool hasComponent() const
            {
                return private_.pointer()->hasComponent<T>();
            }

            template<class T0, class T1> bool hasComponents() const
            {
                return private_.pointer()->hasComponents<T0, T1>();
            }

            template<class T0, class T1, class T2> bool hasComponents() const
            {
                return private_.pointer()->hasComponents<T0, T1, T2>();
            }

            bool hasComponentWithId(const std::vector<unsigned int> & ids);
            unsigned int id() const;
            unsigned int referenceCount() const;

        protected:
            Node(const ReferencedPointer<NodePrivate> &);

        private:
            ReferencedPointer<NodePrivate> private_;
        };
    }
}

#endif

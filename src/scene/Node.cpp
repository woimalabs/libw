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

#include "w/scene/NodePrivate.hpp"
#include "w/scene/Visitor.hpp"
#include "w/scene/Node.hpp"
#include "w/base/Class.hpp"
#include "w/base/Lock.hpp"
#include "w/base/Log.hpp"

namespace w
{
    namespace scene
    {
        Node::Node():
            private_(ReferencedPointer<NodePrivate>(new NodePrivate()))
        {
        }

        Node::Node(const ReferencedPointer<NodePrivate> & private__):
            private_(private__)
        {
        }

        Node::~Node()
        {
        }

        Node::Node(const Node & r):
            private_(r.private_)
        {
        }

        Node& Node::operator=(const Node & r)
        {
            if (this != &r)
            {
                private_ = r.private_;
            }
            return *this;
        }

        Node::Node(const Component & c0):
            private_(ReferencedPointer<NodePrivate>(new NodePrivate(c0)))
        {
        }

        Node::Node(const Component & c0, const Component & c1):
            private_(ReferencedPointer<NodePrivate>(new NodePrivate(c0, c1)))
        {
        }

        Node::Node(const Component & c0, const Component & c1, const Component & c2):
            private_(ReferencedPointer<NodePrivate>(new NodePrivate(c0, c1, c2)))
        {
            if(private_.pointer()->referenceCount() > 1)
            {
                LOGD("WTF Node::id: %d", private_.pointer()->id());
            }
        }

        Node::Node(const Component & c0, const Component & c1, const Component & c2, const Component & c3):
            private_(ReferencedPointer<NodePrivate>(new NodePrivate(c0, c1, c2, c3)))
        {
        }

        void Node::accept(Visitor& visitor)
        {
            if(visitor.canVisit(*this) == true)
            {
                visitor.enter(*this);
                if(visitor.breaking() == false)
                {
                    Node dummy;
                    std::vector<ReferencedPointer<NodePrivate> > tmp = private_.pointer()->children();
                    for (std::vector<ReferencedPointer<NodePrivate> >::iterator i = tmp.begin(); i != tmp.end(); i++)
                    {
                        ReferencedPointer<NodePrivate> tmp2 = *i;
                        dummy.private_ = tmp2.pointer();
                        dummy.accept(visitor);
                    }
                }
                visitor.leave(*this);
            }
        }

        void Node::addChild(Node const& node)
        {
            private_.pointer()->addChild(node.private_.pointer());
        }

        /*void Node::removeChild(Node const& node)
        {
            private_.pointer()->removeChild(node.private_.pointer());
        }

        void Node::removeChildren(std::vector<unsigned int> & ids)
        {
            private_.pointer()->removeChildren(ids);
        }*/

        void Node::addComponent(Component const& component)
        {
            private_.pointer()->addComponent(component);
        }

        void Node::removeComponent(Component const& component)
        {
            private_.pointer()->removeComponent(component);
        }

        void Node::removeComponent(std::string const& type)
        {
            private_.pointer()->removeComponent(type);
        }

        void Node::removeChildWithComponentId(bool recursive, const std::vector<unsigned int> & ids)
        {
            if(ids.size() > 0)
            {
                for(std::vector<unsigned int>::const_iterator i = ids.begin(); i != ids.end(); i++)
                {
                    LOGD("  -id: %d", *i);
                }
            }
            private_.pointer()->removeChildWithComponentId(recursive, ids);
        }

        Component Node::component(std::string const& type)
        {
            ReferencedPointer<ComponentPrivate> tmp = private_.pointer()->component(type);
            if (tmp.pointer() != NULL)
            {
                return Component(tmp.pointer());
            }
            else
            {
                return ComponentNull;
            }
        }

        ReferencedPointer<ComponentPrivate> Node::componentPrivate(std::string const& type)
        {
            return private_.pointer()->component(type);
        }

        template<class T> ReferencedPointer<T> Node::component() const
        {
            return private_.pointer()->component<T>();
        }

        unsigned int Node::id() const
        {
            return private_.pointer()->id();
        }

        unsigned int Node::referenceCount() const
        {
            return private_.pointer()->referenceCount();
        }

        /*std::vector<Node> Node::children()
        {
            std::vector<Node> r;
            return r; //private_.pointer()->children();
        }

        Node Node::parent()
        {
            return NodeNull;
        }*/
    }
}

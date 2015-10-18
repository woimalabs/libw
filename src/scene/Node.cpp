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
            ReferencedPointer<NodePrivate>(new NodePrivate())
        {
        }

        Node::Node(const ReferencedPointer<NodePrivate> & private__):
            ReferencedPointer<NodePrivate>(private__)
        {
        }

        Node::~Node()
        {
        }

        Node::Node(const Node & r):
            ReferencedPointer<NodePrivate>(r)
        {
        }

        Node& Node::operator=(const Node & r)
        {
            ReferencedPointer::operator=(r);
            return *this;
        }

        Node::Node(const Component & c0):
            ReferencedPointer<NodePrivate>(new NodePrivate(c0))
        {
        }

        Node::Node(const Component & c0, const Component & c1):
            ReferencedPointer<NodePrivate>(new NodePrivate(c0, c1))
        {
        }

        Node::Node(const Component & c0, const Component & c1, const Component & c2):
            ReferencedPointer<NodePrivate>(new NodePrivate(c0, c1, c2))
        {
        }

        Node::Node(const Component & c0, const Component & c1, const Component & c2, const Component & c3):
            ReferencedPointer<NodePrivate>(new NodePrivate(c0, c1, c2, c3))
        {
        }

        Node::Node(std::string const& name, const Component & c0):
            ReferencedPointer<NodePrivate>(new NodePrivate(name, c0))
        {
        }

        Node::Node(std::string const& name, const Component & c0, const Component & c1):
            ReferencedPointer<NodePrivate>(new NodePrivate(name, c0, c1))
        {
        }

        Node::Node(std::string const& name, const Component & c0, const Component & c1, const Component & c2):
            ReferencedPointer<NodePrivate>(new NodePrivate(name, c0, c1, c2))
        {
        }

        Node::Node(std::string const& name, const Component & c0, const Component & c1, const Component & c2, const Component & c3):
            ReferencedPointer<NodePrivate>(new NodePrivate(name, c0, c1, c2, c3))
        {
        }

        void Node::accept(Visitor& visitor)
        {
            if(visitor.canVisit(*this) == true)
            {
                visitor.enter(*this);
                if(visitor.breaking() == false)
                {
                    //Node dummy;
                    std::vector<ReferencedPointer<NodePrivate> > tmp = pointer()->children();
                    for (std::vector<ReferencedPointer<NodePrivate> >::iterator i = tmp.begin(); i != tmp.end(); i++)
                    {
                        ReferencedPointer<NodePrivate> tmp2 = *i;
                        Node dummy(tmp2.pointer());
                        dummy.accept(visitor);
                    }
                }
                visitor.leave(*this);
            }
        }

        void Node::addChild(Node const& node)
        {
            pointer()->addChild(node.pointer());
        }

        void Node::addComponent(Component const& component)
        {
            pointer()->addComponent(component);
        }

        bool Node::hasComponents() const
        {
            return pointer()->hasComponents();
        }

        void Node::removeChildWithComponentId(bool recursive, const std::vector<unsigned int> & ids)
        {
            pointer()->removeChildWithComponentId(recursive, ids);
        }
        
        void Node::removeChildWithId(bool recursive, unsigned int id)
        {
            pointer()->removeChildWithId(recursive, id);
        }

        void Node::removeChildren()
        {
            pointer()->removeChildren();
        }

        bool Node::hasComponentWithId(const std::vector<unsigned int> & ids)
        {
            return pointer()->hasComponentWithId(ids);
        }

        unsigned int Node::id() const
        {
            return pointer()->id();
        }

        unsigned int Node::referenceCount() const
        {
            return pointer()->referenceCount();
        }

        const std::string Node::name() const
        {
            return pointer()->name();
        }
    }
}

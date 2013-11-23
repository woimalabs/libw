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

#include "NodePrivate.hpp"
#include "w/scene/Visitor.hpp"
#include "w/scene/Node.hpp"
#include "w/base/Class.hpp"
#include "w/base/Lock.hpp"

namespace w
{
    namespace scene
    {
        Node::Node():
            private_(ReferencedPointer<NodePrivate>(new NodePrivate()))
        {
        }

        Node::~Node()
        {
        }

        Node::Node(Node const& r):
            private_(r.private_)
        {
        }

        Node& Node::operator=(Node const& r)
        {
            private_ = r.private_;
            return *this;
        }

        Node::Node(Component const& c0):
            private_(ReferencedPointer<NodePrivate>(new NodePrivate(c0)))
        {
        }

        Node::Node(Component const& c0, Component const& c1):
            private_(ReferencedPointer<NodePrivate>(new NodePrivate(c0, c1)))
        {
        }

        Node::Node(Component const& c0, Component const& c1, Component const& c2):
            private_(ReferencedPointer<NodePrivate>(new NodePrivate(c0, c1, c2)))
        {
        }

        Node::Node(Component const& c0, Component const& c1, Component const& c2, Component const& c3):
            private_(ReferencedPointer<NodePrivate>(new NodePrivate(c0, c1, c2, c3)))
        {
        }

        void Node::accept(Visitor& visitor)
        {
            visitor.enter(*this);
            std::vector<ReferencedPointer<NodePrivate> > tmp = private_.pointer()->children();
            for (std::vector<ReferencedPointer<NodePrivate> >::iterator i = tmp.begin(); i != tmp.end(); i++)
            {
                ReferencedPointer<NodePrivate> tmp2 = *i;
                Node tmp3;
                tmp3.private_ = tmp2.pointer();
                tmp3.accept(visitor);
            }
            visitor.leave(*this);
        }

        void Node::addChild(Node const& node)
        {
            private_.pointer()->addChild(node.private_.pointer());
        }

        void Node::addComponent(Component const& component)
        {
            private_.pointer()->addComponent(component);
        }

        void Node::removeComponent(Component const& component)
        {
            private_.pointer()->removeComponent(component);
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

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
#include "w/scene/Node.hpp"
#include <w/base/Log.hpp>
#include "w/base/Lock.hpp"

namespace w
{
    namespace scene
    {
        NodePrivate::NodePrivate():
            parent_(NULL)
        {
        }

        NodePrivate::NodePrivate(Component const& c0):
            parent_(NULL)
        {
            addComponent(c0);
        }

        NodePrivate::NodePrivate(Component const& c0, Component const& c1):
            parent_(NULL)
        {
            addComponent(c0);
            addComponent(c1);
        }

        NodePrivate::NodePrivate(Component const& c0, Component const& c1, Component const& c2):
            parent_(NULL)
        {
            addComponent(c0);
            addComponent(c1);
            addComponent(c2);
        }

        NodePrivate::NodePrivate(Component const& c0, Component const& c1, Component const& c2, Component const& c3):
            parent_(NULL)
        {
            addComponent(c0);
            addComponent(c1);
            addComponent(c2);
            addComponent(c3);
        }

        NodePrivate::~NodePrivate()
        {
            LOCK_(mutexStructure_);

            for (std::list<NodePrivate*>::iterator i = children_.begin(); i != children_.end(); i++)
            {
                NodePrivate* tmp = *i;
                tmp->parent_ = NULL;
                tmp->decrement();
                tmp = NULL;
            }
        }

        void NodePrivate::removeComponent(Component const& component)
        {
            LOCK_(mutexComponents_);

            std::string key = component.type();
            std::map<std::string, ReferencedPointer<ComponentPrivate> >::iterator i;
            i = components_.find(key);

            if (i == components_.end())
            {
                LOGE("NodePrivate::removeComponent(), did not have component with type: \"%s\", doing nothing.", key.c_str());
            }
            else
            {
                components_.erase(i);
            }
        }

        void NodePrivate::removeComponent(std::string const& type)
        {
            LOCK_(mutexComponents_);

            std::map<std::string, ReferencedPointer<ComponentPrivate> >::iterator i = components_.find(type);

            if (i == components_.end())
            {
                LOGE("NodePrivate::removeComponent(), did not have component with type: \"%s\", doing nothing.", type.c_str());
            }
            else
            {
                components_.erase(i);
            }
        }

        void NodePrivate::addComponent(Component const& component)
        {
            LOCK_(mutexComponents_);

            std::string key = component.type();
            std::map<std::string, ReferencedPointer<ComponentPrivate> >::iterator i;
            i = components_.find(key);

            if (i == components_.end())
            {
                std::pair<std::string, ReferencedPointer<ComponentPrivate> > tmp = std::make_pair(key, component.private_);
                components_.insert(tmp);
            }
            else
            {
                // We had this component already
                LOGE("NodePrivate::addComponent(), trying to overwrite existing component with type: \"%s\", doing nothing.", key.c_str());
            }
        }

        ReferencedPointer<ComponentPrivate> NodePrivate::component(std::string const& type)
        {
            LOCK_(mutexComponents_);

            std::map<std::string, ReferencedPointer<ComponentPrivate> >::iterator i = components_.find(type);
            if (i != components_.end())
            {
                return (i->second);
            }
            else
            {
                return ReferencedPointer<ComponentPrivate>(NULL);
            }
        }

        template<class T> ReferencedPointer<T> NodePrivate::component()
        {
            LOCK_(mutexComponents_);

            std::map<std::string, ReferencedPointer<ComponentPrivate> >::iterator i;
            for(i = components_.begin(); i != components_.end(); ++i)
            {
                ReferencedPointer<ComponentPrivate> tmp = i->second;
                T* tmp2 = dynamic_cast<T*>(tmp.pointer());
                if(tmp2 != NULL)
                {
                    return ReferencedPointer<T>(tmp2);
                }
            }

            return ReferencedPointer<T>(NULL);
        }

        void NodePrivate::addChild(NodePrivate* node)
        {
            LOCK_(mutexStructure_);

            children_.push_back(node);
            node->parent_ = this;
            node->increment();
        }

        ReferencedPointer<NodePrivate> NodePrivate::parent()
        {
            LOCK_(mutexStructure_);

            return ReferencedPointer<NodePrivate>(parent_);
        }

        std::vector<ReferencedPointer<NodePrivate> > NodePrivate::children()
        {
            LOCK_(mutexStructure_);

            std::vector<ReferencedPointer<NodePrivate> > r;
            for(std::list<NodePrivate*>::iterator i = children_.begin(); i != children_.end(); i++)
            {
                r.push_back(ReferencedPointer<NodePrivate>(*i));
            }
            return r;
        }
    }
}

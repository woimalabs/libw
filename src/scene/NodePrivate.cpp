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
            name_(),
            parent_(NULL)
        {
            addComponent(c0);
        }

        NodePrivate::NodePrivate(Component const& c0, Component const& c1):
            name_(),
            parent_(NULL)
        {
            addComponent(c0);
            addComponent(c1);
        }

        NodePrivate::NodePrivate(Component const& c0, Component const& c1, Component const& c2):
            name_(),
            parent_(NULL)
        {
            addComponent(c0);
            addComponent(c1);
            addComponent(c2);
        }

        NodePrivate::NodePrivate(Component const& c0, Component const& c1, Component const& c2, Component const& c3):
            name_(),
            parent_(NULL)
        {
            addComponent(c0);
            addComponent(c1);
            addComponent(c2);
            addComponent(c3);
        }

        NodePrivate::NodePrivate(std::string const& name, Component const& c0):
            name_(name),
            parent_(NULL)
        {
            addComponent(c0);
        }

        NodePrivate::NodePrivate(std::string const& name, Component const& c0, Component const& c1):
            name_(name),
            parent_(NULL)
        {
            addComponent(c0);
            addComponent(c1);
        }

        NodePrivate::NodePrivate(std::string const& name, Component const& c0, Component const& c1, Component const& c2):
            name_(name),
            parent_(NULL)
        {
            addComponent(c0);
            addComponent(c1);
            addComponent(c2);
        }

        NodePrivate::NodePrivate(std::string const& name, Component const& c0, Component const& c1, Component const& c2, Component const& c3):
            name_(name),
            parent_(NULL)
        {
            addComponent(c0);
            addComponent(c1);
            addComponent(c2);
            addComponent(c3);
        }

        NodePrivate::~NodePrivate()
        {
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

            {
                LOCK_(mutexComponents_);
                while(components_.size() > 0)
                {
                    components_.erase(components_.begin());
                }
            }
        }

        void NodePrivate::removeChildWithComponentId(bool recursive, const std::vector<unsigned int> & ids)
        {
            LOCK_(mutexStructure_);

            for(std::list<NodePrivate*>::iterator i = children_.begin(); i != children_.end();)
            {
                NodePrivate* tmp = *i;
                if(tmp->hasComponentWithId(ids))
                {
                    tmp->parent_ = NULL;
                    tmp->decrement();
                    tmp = NULL;
                    i = children_.erase(i);
                }
                else
                {
                    tmp->removeChildWithComponentId(recursive, ids);
                    ++i;
                }
            }
        }

        bool NodePrivate::hasComponentWithId(const std::vector<unsigned int> & ids)
        {
            LOCK_(mutexComponents_);

            for(std::map<const std::type_info*, ReferencedPointer<ComponentPrivate> >::iterator i = components_.begin();
                i != components_.end();
                i++)
            {
                ReferencedPointer<ComponentPrivate> tmp = i->second;
                if(tmp.isNull() == false)
                {
                    unsigned int id = tmp.pointer()->id();
                    for(std::vector<unsigned int>::const_iterator i2 = ids.begin(); i2 != ids.end(); i2++)
                    {
                        if(id == *i2)
                        {
                            return true;
                        }
                    }
                }
            }
            return false;
        }

        void NodePrivate::removeChildren()
        {
            LOCK_(mutexStructure_);

            for(std::list<NodePrivate*>::iterator i = children_.begin(); i != children_.end();)
            {
                NodePrivate* tmp = *i;

                // Recursion
                tmp->removeChildren();

                // Iterator child
                tmp->parent_ = NULL;
                tmp->decrement();
                tmp = NULL;
                i = children_.erase(i);
            }
        }
        
        void NodePrivate::removeChildWithId(bool recursive, unsigned int id)
        {
            LOCK_(mutexStructure_);
            
            for(std::list<NodePrivate*>::iterator i = children_.begin(); i != children_.end();)
            {
                NodePrivate* tmp = *i;
                if(tmp->id() == id)
                {
                    tmp->parent_ = NULL;
                    tmp->decrement();
                    tmp = NULL;
                    i = children_.erase(i);
                }
                else if(recursive)
                {
                    tmp->removeChildWithId(recursive, id);
                    ++i;
                }
            }
        }
        
        void NodePrivate::removeChildrenWithName(bool recursive, const std::string& name)
        {
            LOCK_(mutexStructure_);
            
            for(std::list<NodePrivate*>::iterator i = children_.begin(); i != children_.end();)
            {
                NodePrivate* tmp = *i;
                if(tmp->name() == name)
                {
                    tmp->parent_ = NULL;
                    tmp->decrement();
                    tmp = NULL;
                    i = children_.erase(i);
                }
                else if(recursive)
                {
                    tmp->removeChildrenWithName(recursive, name);
                    ++i;
                }
            }
        }
        
        void NodePrivate::addComponent(Component const& component)
        {
            LOCK_(mutexComponents_);

            const std::type_info& key = component.typeId();
            std::map<const std::type_info*, ReferencedPointer<ComponentPrivate> >::iterator i = components_.find(&key);
            if(i == components_.end())
            {
                std::pair<const std::type_info*, ReferencedPointer<ComponentPrivate> > tmp = std::make_pair(&key, component.private_);
                components_.insert(tmp);
            }
            else
            {
                // We had this component already
                LOGE("NodePrivate::addComponent(), trying to overwrite existing component with type. Doing nothing.");
            }
        }

        bool NodePrivate::hasComponents() const
        {
            return components_.size() != 0;
        }
        
        bool NodePrivate::hasParent() const
        {
            return parent_ != NULL;
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

        const std::string NodePrivate::name() const
        {
            return name_;
        }
    }
}

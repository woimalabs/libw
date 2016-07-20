/**
 * libw
 *
 * Copyright (C) 2013-2016 Woima Solutions
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
#include "w/base/Log.hpp"
#include "w/base/Lock.hpp"
#include "w/base/String.hpp"
#include <set>

namespace w
{
    namespace scene
    {
        std::map<
            unsigned int, 
            std::map<
                const std::type_info*,
                std::set<NodePrivate*>
                > 
            > NodePrivate::treeComponentNodes_;

        NodePrivate::NodePrivate():
            parent_(NULL),
            treeId_(0)
        {
        }

        NodePrivate::NodePrivate(Component const& c0):
            name_(),
            parent_(NULL),
            treeId_(0)
        {
            addComponent(c0);
        }

        NodePrivate::NodePrivate(Component const& c0, Component const& c1):
            name_(),
            parent_(NULL),
            treeId_(0)
        {
            addComponent(c0);
            addComponent(c1);
        }

        NodePrivate::NodePrivate(Component const& c0, Component const& c1, Component const& c2):
            name_(),
            parent_(NULL),
            treeId_(0)
        {
            addComponent(c0);
            addComponent(c1);
            addComponent(c2);
        }

        NodePrivate::NodePrivate(Component const& c0, Component const& c1, Component const& c2, Component const& c3):
            name_(),
            parent_(NULL),
            treeId_(0)
        {
            addComponent(c0);
            addComponent(c1);
            addComponent(c2);
            addComponent(c3);
        }

        NodePrivate::NodePrivate(Component const& c0, Component const& c1, Component const& c2, Component const& c3, Component const& c4):
            name_(),
            parent_(NULL),
            treeId_(0)
        {
            addComponent(c0);
            addComponent(c1);
            addComponent(c2);
            addComponent(c3);
            addComponent(c4);
        }

        NodePrivate::NodePrivate(std::string const& name):
            name_(name),
            parent_(NULL),
            treeId_(0)
        {
        }

        NodePrivate::NodePrivate(std::string const& name, Component const& c0):
            name_(name),
            parent_(NULL),
            treeId_(0)
        {
            addComponent(c0);
        }

        NodePrivate::NodePrivate(std::string const& name, Component const& c0, Component const& c1):
            name_(name),
            parent_(NULL),
            treeId_(0)
        {
            addComponent(c0);
            addComponent(c1);
        }

        NodePrivate::NodePrivate(std::string const& name, Component const& c0, Component const& c1, Component const& c2):
            name_(name),
            parent_(NULL),
            treeId_(0)
        {
            addComponent(c0);
            addComponent(c1);
            addComponent(c2);
        }

        NodePrivate::NodePrivate(std::string const& name, Component const& c0, Component const& c1, Component const& c2, Component const& c3):
            name_(name),
            parent_(NULL),
            treeId_(0)
        {
            addComponent(c0);
            addComponent(c1);
            addComponent(c2);
            addComponent(c3);
        }

        NodePrivate::NodePrivate(std::string const& name, Component const& c0, Component const& c1, Component const& c2, Component const& c3, Component const& c4):
            name_(name),
            parent_(NULL),
            treeId_(0)
        {
            addComponent(c0);
            addComponent(c1);
            addComponent(c2);
            addComponent(c3);
            addComponent(c4);
        }


        NodePrivate::~NodePrivate()
        {
            {
                for (std::list<NodePrivate*>::iterator i = children_.begin(); i != children_.end(); i++)
                {
                    NodePrivate* tmp = *i;
                    tmp->parent_ = NULL;
                    tmp->decrement();
                    tmp = NULL;
                }
            }

            {
                while(components_.size() > 0)
                {
                    const std::type_info& key = components_.begin()->second.pointer()->typeId();
                    treeComponentNodesRemove(treeId_, key, this);
                    components_.erase(components_.begin());
                }
            }
        }

        void NodePrivate::removeChildWithComponentId(bool recursive, const std::vector<unsigned int> & ids)
        {
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
            const std::type_info& key = component.typeId();
            std::map<const std::type_info*, ReferencedPointer<ComponentPrivate> >::iterator i = components_.find(&key);
            if(i == components_.end())
            {
                std::pair<const std::type_info*, ReferencedPointer<ComponentPrivate> > tmp = std::make_pair(&key, component.private_);
                components_.insert(tmp);

                // component node cache management
                treeComponentNodesAdd(treeId_, key, this);
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
            children_.push_back(node);
            node->parent_ = this;

            // change node's components to new tree    
            if(node->treeId_ != treeId_)
            {
                node->setTreeId(treeId_);
                /*
                //LOGD("chanching tree for all components, node: %d", node->id());
                for(auto i: node->components_)
                {
                    const std::type_info& key = i.second.pointer()->typeId();
                    treeComponentNodesRemove(node->treeId_, key, node); 
                    treeComponentNodesAdd(treeId_, key, node);
                }  
                node->treeId_ = treeId_;*/
            }
            node->increment();
        }

        ReferencedPointer<NodePrivate> NodePrivate::parent()
        {
            return ReferencedPointer<NodePrivate>(parent_);
        }

        std::vector<ReferencedPointer<NodePrivate> > NodePrivate::children()
        {
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

        void NodePrivate::setTreeId(unsigned int value)
        {
            //LOGD("setTreeId %d", value);

            unsigned int oldTreeId = treeId_;
            treeId_ = value;

            // change node's components to new tree    
            for(auto i: components_)
            {
                const std::type_info& key = i.second.pointer()->typeId();
                treeComponentNodesRemove(oldTreeId, key, this);
                treeComponentNodesAdd(treeId_, key, this);
            }

            // change recursively whole tree branch
            for(auto i: children_)
            {
                LOG
                i->setTreeId(value);
            }  
        }

        unsigned int NodePrivate::treeId() const
        {
            return treeId_;
        }        

        void NodePrivate::printTreeComponentNode()
        {
            LOGD("### printTreeComponentNode, start ###");
            unsigned int treeAmount = 0;
            std::set<std::string> componentTypes;
            std::set<unsigned int> totalNodeAmount;

            for(auto trees: treeComponentNodes_)
            {
                LOGD("tree: %d", trees.first);
                treeAmount++;

                for(auto submap: trees.second)
                {
                    componentTypes.insert(submap.first->name());
                    std::string tmp = "";
                    std::set<unsigned int> subNodeAmount;

                    for(auto i: submap.second)
                    {
                        tmp += w::String::toString(i->id()) + " ";
                        totalNodeAmount.insert(i->id());
                        subNodeAmount.insert(i->id());
                    }                    
                    LOGD("- submap %s: nodes (count: %d): %s", submap.first->name(), subNodeAmount.size(), tmp.c_str());
                }
            }

            LOGD("view amount: %d", treeAmount);
            LOGD("component types in use: %d", componentTypes.size());
            LOGD("total node amount: %d", totalNodeAmount.size());
            LOGD("### printTreeComponentNode, end ###");
        }

        void NodePrivate::treeComponentNodesAdd(unsigned int treeId, const std::type_info& key, NodePrivate* node)
        {
            auto i = treeComponentNodes_.find(treeId);
            if(i == treeComponentNodes_.end()) // no tree, create new tree and submap for caching
            {
                //LOGD("adding0 tree: %d, list: %s, node: %d", treeId, key.name(), node->id());
                std::set<NodePrivate*> nodes;
                nodes.insert(node);
                std::map<const std::type_info*, std::set<NodePrivate*> > submap;
                submap.insert(std::make_pair(&key, nodes));

                treeComponentNodes_.insert(std::make_pair(treeId, submap));
            }
            else // tree found
            {
                auto j = i->second.find(&key);
                if(j == i->second.end()) // no submap for component type in tree, create it
                {
                    //LOGD("adding1 tree: %d, list: %s, node: %d", treeId, key.name(), node->id());
                    std::set<NodePrivate*> nodes;
                    nodes.insert(node);
                    i->second.insert(std::make_pair(&key, nodes));
                }
                else // add to existing component key submap
                {
                    //LOGD("adding2 tree: %d, list: %s, node: %d", treeId, key.name(), node->id());
                    auto iter = j->second.find(node);
                    if(iter != j->second.end()) 
                    {
                        printTreeComponentNode();
                        LOGE("add, tree: %d, list: %s, node: %d", treeId, key.name(), node->id());
                        throw Exception("Had already added");
                    }
                    j->second.insert(node);
                }
            }
        }
        void NodePrivate::treeComponentNodesRemove(unsigned int treeId, const std::type_info& key, NodePrivate* node)
        {
            auto i = treeComponentNodes_.find(treeId);
            if(i == treeComponentNodes_.end()) // no tree, corrupted addition / no addition earlier
            {
                //LOGD("rem0 to tree: %d, list: %s, node: %d", treeId, key.name(), node->id());
                printTreeComponentNode();
                throw Exception("Removed component did not exist in tree. No tree when removing.");
            }
            else // tree found
            {
                auto j = i->second.find(&key);
                if(j == i->second.end()) // no list, corrupted addition / no addition earlier
                {
                    //LOGD("rem1 to tree: %d, list: %s, node: %d", treeId, key.name(), node->id());
                    printTreeComponentNode();
                    throw Exception("Removed component did not exist in tree. No key list when removing.");
                }
                else // remove from list
                {
                    //LOGD("rem2 to tree: %d, list: %s, node: %d", treeId, key.name(), node->id());
                    j->second.erase(node);
                    auto iter = j->second.find(node);
                    if(iter != j->second.end()) 
                    {
                        printTreeComponentNode();
                        LOGE("rem, tree: %d, list: %s, node: %d", treeId, key.name(), node->id());
                        throw Exception("Remove failed");
                    }
                }
            }
        }
    }
}

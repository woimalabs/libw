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

#ifndef LIBW_SCENE_NODEPRIVATE
#define LIBW_SCENE_NODEPRIVATE

#include "w/scene/Visitor.hpp"
#include "w/scene/Component.hpp"
#include <w/base/Class.hpp>
#include "w/base/ReferencedPointer.hpp"
#include "w/base/Referenced.hpp"
#include <map>
#include <list>
#include <set>
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
            NodePrivate(Component const& c0, Component const& c1, Component const& c2, Component const& c3, Component const& c4);
            NodePrivate(std::string const& name);
            NodePrivate(std::string const& name, Component const& c0);
            NodePrivate(std::string const& name, Component const& c0, Component const& c1);
            NodePrivate(std::string const& name, Component const& c0, Component const& c1, Component const& c2);
            NodePrivate(std::string const& name, Component const& c0, Component const& c1, Component const& c2, Component const& c3);
            NodePrivate(std::string const& name, Component const& c0, Component const& c1, Component const& c2, Component const& c3, Component const& c4);

            virtual ~NodePrivate();
            void addComponent(Component const& component);
            bool hasComponents() const;

            template<class T> void removeComponent()
            {
                const std::type_info& key = typeid(T);
                std::map<const std::type_info*, ReferencedPointer<ComponentPrivate> >::iterator i = components_.find(&key);
                if(i == components_.end())
                {
                    LOGD("NodePrivate::removeComponent(), did not have component with type. Doing nothing");;
                }
                else
                {
                    treeComponentNodesRemove(treeId_, key, this);
                    components_.erase(i);
                }
            }

            template<class T> T* component()
            {
                const std::type_info& key = typeid(T);
                std::map<const std::type_info*, ReferencedPointer<ComponentPrivate> >::const_iterator i = components_.find(&key);
                if(i != components_.end())

                {
                    return static_cast<T*>(i->second.pointer());
                }
                return NULL;
            }

            template<class T> bool hasComponent() const
            {
                const std::type_info& key = typeid(T);
                std::map<const std::type_info*, ReferencedPointer<ComponentPrivate> >::const_iterator i = components_.find(&key);
                if(i != components_.end())
                {
                    return true;
                }
                return false;
            }

            template<class T0, class T1> bool hasComponents() const
            {
                const std::type_info& key0 = typeid(T0);
                std::map<const std::type_info*, ReferencedPointer<ComponentPrivate> >::const_iterator i0 = components_.find(&key0);
                const std::type_info& key1 = typeid(T1);
                std::map<const std::type_info*, ReferencedPointer<ComponentPrivate> >::const_iterator i1 = components_.find(&key1);
                if(i0 != components_.end() && i1 != components_.end())
                {
                    return true;
                }
                return false;
            }

            template<class T0, class T1, class T2> bool hasComponents() const
            {
                const std::type_info& key0 = typeid(T0);
                std::map<const std::type_info*, ReferencedPointer<ComponentPrivate> >::const_iterator i0 = components_.find(&key0);
                const std::type_info& key1 = typeid(T1);
                std::map<const std::type_info*, ReferencedPointer<ComponentPrivate> >::const_iterator i1 = components_.find(&key1);
                const std::type_info& key2 = typeid(T2);
                std::map<const std::type_info*, ReferencedPointer<ComponentPrivate> >::const_iterator i2 = components_.find(&key2);
                if(i0 != components_.end() && i1 != components_.end() && i2 != components_.end())
                {
                    return true;
                }
                return false;
            }

            bool hasParent() const;
            void addChild(NodePrivate* node);
            void removeChildWithComponentId(bool recursive, const std::vector<unsigned int> & ids);
            bool hasComponentWithId(const std::vector<unsigned int> & ids);
            void removeChildren();
            void removeChildWithId(bool recursive, unsigned int id);
            void removeChildrenWithName(bool recursive, const std::string& name);
            std::vector<ReferencedPointer<NodePrivate> > children();
            ReferencedPointer<NodePrivate> parent();
            const std::string name() const;
            void setTreeId(unsigned int);
            unsigned int treeId() const;

            /* use with care, you should not modify the node & component structure while going through returned list */
            template<class T> std::set<NodePrivate*>* componentNodes()
            {
                const std::type_info& key = typeid(T);
                auto i = treeComponentNodes_.find(treeId_);
                if(i != treeComponentNodes_.end())
                {
                    auto j = i->second.find(&key);
                    if(j != i->second.end()) 
                    {
                        return &(j->second); 
                    }
                }

                return &componentNodesEmpty_;
            }   
            static void printTreeComponentNode();
            static unsigned int totalComponentCount();
            
        private:
            unsigned int treeId_;


            std::string name_;
            std::list<NodePrivate*> children_;
            NodePrivate* parent_;

            std::map<const std::type_info*, ReferencedPointer<ComponentPrivate> > components_;

            /* component nodes tree */
            static void treeComponentNodesAdd(unsigned int treeId, const std::type_info& componentType, NodePrivate*);
            static void treeComponentNodesRemove(unsigned int treeId, const std::type_info& componentType, NodePrivate*);

            static std::map<
                unsigned int, 
                std::map<
                    const std::type_info*,
                    std::set<NodePrivate*>
                    > 
                >
                treeComponentNodes_;

            static std::set<NodePrivate*> componentNodesEmpty_;
        };
    }
}

#endif

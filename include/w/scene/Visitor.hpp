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

#ifndef LIBW_SCENE_VISITOR
#define LIBW_SCENE_VISITOR

#include <w/base/Class.hpp>

namespace w
{
    namespace scene
    {
        class Node;

        /**
        * @class Visitor
        *
        * Visitor for nodes. Inherit to do your magic.
        */
        class Visitor
        {
        public:
            UNCOPYABLE(Visitor)

            Visitor():
                break_(false)
            {
            }

            virtual ~Visitor()
            {
            }

            /**
             * Called before entering the node
             */
            virtual bool canVisit(Node& node)
            {
                return true;
            }

            /**
             * Called when visitor is entering the node.
             */
            virtual void enter(Node& node) = 0;

            /**
             * Called when visitor is leaving the node.
             */
            virtual void leave(Node& node) = 0;

            /**
             * Called when user wants visitor to break out from travelsal.
             * leave() functions will be called normally when coming out from the
             * node tree.
             */
            inline void setBreak()
            {
                break_ = true;
            }

            /**
             * Called when visitor is leaving the node.
             */
            inline bool breaking()
            {
                return break_;
            }
        private:
            bool break_;
        };
    }
}

#endif

/**
 * OpenDaVINCI - Portable middleware for distributed components.
 * Copyright (C) 2008 - 2015 Christian Berger, Bernhard Rumpe
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef OPENDAVINCI_CORE_BASE_TREENODE_H_
#define OPENDAVINCI_CORE_BASE_TREENODE_H_

// core/platform.h must be included to setup platform-dependent header files and configurations.
#include "core/platform.h"

namespace core {
    namespace base {

        using namespace std;

        /**
         * This class represents a generic tree data structure.
         */
        template<typename T>
        class OPENDAVINCI_API TreeNode {
            private:
                /**
                 * "Forbidden" copy constructor. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the copy constructor.
                 */
                TreeNode(const TreeNode &);

                /**
                 * "Forbidden" assignment operator. Goal: The compiler should warn
                 * already at compile time for unwanted bugs caused by any misuse
                 * of the assignment operator.
                 */
                TreeNode& operator=(const TreeNode &);

            public:
                TreeNode() :
                    m_value(),
                    m_parent(NULL),
                    m_children() {}

                virtual ~TreeNode() {
                    typename vector<TreeNode<T>* >::iterator it = m_children.begin();
                    while (it != m_children.end()) {
                        TreeNode<T> *child = (*it++);
                        delete child;
                    }
                    m_children.clear();
                }

                /**
                 * This method sets the value for this tree node.
                 *
                 * @param value Value.
                 */
                void setValue(T value) {
                    m_value = value;
                }

                /**
                 * This method returns the value.
                 *
                 * @return Value for this node.
                 */
                T getValue() {
                    return m_value;
                }

                /**
                 * This method returns the parent node.
                 *
                 * @return Parental node.
                 */
                TreeNode<T>* getParent() {
                    return m_parent;
                }

                /**
                 * This method sets the parent node.
                 *
                 * @param parent Parental node.
                 */
                void setParent(TreeNode<T> *parent) {
                    m_parent = parent;
                }

                /**
                 * This method adds a child.
                 *
                 * @param child Child to be added.
                 */
                void addChild(TreeNode<T> *child) {
                    if (child != NULL) {
                        child->setParent(this);
                        m_children.push_back(child);
                    }
                }

                /**
                 * This method returns the list of children.
                 *
                 * @return List of children.
                 */
                vector<TreeNode<T>* > getChildren() {
                    return m_children;
                }

            private:
                T m_value;
                TreeNode<T> *m_parent;
                vector<TreeNode<T>* > m_children;
        };

    }
} // core::base

#endif /*OPENDAVINCI_CORE_BASE_TREENODE_H_*/

/**
 * @file Tree.h
 * @date Mon, 25th September, 2023
 * @author Siddharth Mishra (admin@brightprogrammer.in)
 * @copyright Copyright 2023 Siddharth Mishra
 * @copyright Copyright 2023 Anvie Labs
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @brief Container for handling generic tree data types.
 * */

#ifndef ANV_UTILS_TREE_H
#define ANV_UTILS_TREE_H

#include <Anvie/Containers/Vector.h>
#include <Anvie/Containers/Common.h>

typedef struct anvie_tree_t AnvTree;
typedef struct anvie_tree_node_t AnvTreeNode;

/**
 * Represents a single node in the whole tree.
 * A node points to multiple other nodes.
 * There are two types of parents due to this organization of data.
 *
 * One is the parent that is root of the whole tree (@c tree_parent). This parent will
 * be tree parent of all it's childrent, grand childrent, grand grand children,
 * and so on...
 *
 * Then there's the absolute parent (@c node_parent), that is above the given node in the
 * heirarcy tree. This parent is different for each node at different heights in the tree.
 *
 * To create copy of data and consult other information such as copy constructor
 * and destructor as well as size of data contained in each node, each node needs
 * to access it's tree parent. This way, we reduce copy of same data again and
 * again for each node.
 *
 * @c node_parent member is there to help traversing up the tree.
 * */
struct anvie_tree_node_t {
    void*        p_data;        /**< Data held by this tree node. */
    AnvVector*   vec_children;  /**< AnvVector<AnvTreeNode> containing children of this node. */
    AnvTree*     tree_parent;   /**< Super duper parent of this tree. This is parent of all tree nodes in itself. */
    AnvTreeNode* node_parent;   /**< Absolute parent of this tree node. This is just above this node in heirarchy. */
};
#define TO_ANV_TREE_NODE(x) ((AnvTreeNode*)(x))

void anv_tree_node_create_copy(void* p_dst, void* p_src);
void anv_tree_node_destroy_copy(void* p_copy);
DEF_ANV_STRUCT_VECTOR_INTERFACE(tree_node, AnvTreeNode, anv_tree_node_create_copy, anv_tree_node_destroy_copy);

Size anv_tree_node_get_size(AnvTreeNode* node);
Size anv_tree_node_get_height(AnvTreeNode* node);

AnvTreeNode* anv_tree_node_add(AnvTreeNode* node, void* p_data);
AnvTreeNode* anv_tree_node_remove(AnvTreeNode* node, Size index);
void anv_tree_node_delete(AnvTreeNode* node, Size index);

/**
 * Represents a node in tree.
 * Internal working of this is almost similar to that of
 * AnvVector. We use copy constructors and destructors to
 *
 * Tree inherits from AnvTreeNode in order to behave like it, but
 * it has slightly more elements that the node itself.
 * Each node will refer to this tree for constructing itself!
 * This is to get information about how much data to copy
 * and whether to use copy constructors and destructors
 * for destruction and construction of data.
 * */
struct anvie_tree_t {
    AnvTreeNode                   node_root; /**< Root node of the whole tree. */
    Size                          element_size; /**< Size of p_data element. */
    AnvCreateElementCopyCallback  pfn_create_copy; /**< Copy constructor for each element. Can be NULL. */
    AnvDestroyElementCopyCallback pfn_destroy_copy; /**< Copy destructor for each element. Can be NULL. */
};
#define TO_ANV_TREE(x) ((AnvTree*)(x))

/**
 * Create a new tree object.
 * Like vectors, you either have both @c pfn_create_copy
 * and @c pfn_destroy_copy or you don't have them at all.
 *
 * @param element_size Size of data type to be stored in this tree.
 * @param pfn_create_copy Copy constructor callback for creating copy
 * of each data inserted into the tree while construction of nodes.
 * This way, tree creates a copy of data each time it's added to it,
 * and hence avoids lots of very common memory bugs.
 * @param pfn_destroy_copy Copy destructor callback for destroying
 * copies created using @c pfn_create_copy.
 *
 * @return New created tree.
 * */
FORCE_INLINE AnvTree* anv_tree_create (
    Size element_size,
    AnvCreateElementCopyCallback pfn_create_copy,
    AnvDestroyElementCopyCallback pfn_destroy_copy
) {
    RETURN_VALUE_IF_FAIL(element_size, NULL, ERR_INVALID_ARGUMENTS);

    Bool b1 = pfn_create_copy != NULL;
    Bool b2 = pfn_destroy_copy != NULL;
    RETURN_VALUE_IF_FAIL(!(b1 ^ b2), NULL,
                         "Either both copy constructor and destructor should be NULL, "
                         "or both should be non NULL at the same time!");

    AnvTree* tree = NEW(AnvTree);
    RETURN_VALUE_IF_FAIL(tree, NULL, ERR_OUT_OF_MEMORY);

    tree->element_size          = element_size;
    tree->pfn_create_copy       = pfn_create_copy;
    tree->pfn_destroy_copy      = pfn_destroy_copy;
    tree->node_root.tree_parent = tree;
    tree->node_root.node_parent = TO_ANV_TREE_NODE(tree);

    return tree;
}

/**
 * Destroy given tree.
 *
 * @param tree AnvTree* to be destroyed.
 * */
FORCE_INLINE void anv_tree_destroy(AnvTree* tree) {
    anv_tree_node_destroy_copy(tree);
    FREE(tree);
}

/**
 * Get size of tree. Size is the total number
 * of chilren, grand children, grand grand children and so on...
 * in the given tree.
 *
 * @param tree AnvTree* to get size of.
 *
 * @return Size of given tree.
 * */
FORCE_INLINE Size anv_tree_get_size(AnvTree* tree) {
    return anv_tree_node_get_size(TO_ANV_TREE_NODE(tree));
}

/**
 * Get height of this tree.
 * This is computed everytime the tree is called!
 *
 * @param tree AnvTree* object to get height of.
 *
 * @return Height of given tree object.
 * */
FORCE_INLINE Size anv_tree_get_height(AnvTree* tree) {
    return anv_tree_node_get_height(TO_ANV_TREE_NODE(tree));
}

/**
 * Add a child node to given tree.
 *
 * @param tree AnvTree to insert new child node into.
 * @param p_data Pointer to data to be set as child node's data.
 *
 * @param AnvTreeNode* of new created child node on success.
 * NULL otherwise.
 * */
FORCE_INLINE AnvTreeNode* anv_tree_add(AnvTree* tree, void* p_data) {
    return anv_tree_node_add(TO_ANV_TREE_NODE(tree), p_data);
}

/**
 * Remove a child from tree.
 * Unlike deletion, remove returns the pointer to now removed
 * child node from tree.
 *
 * This is also different from how @c AnvVector work.
 * In case of @c AnvVector, when an element is returned, the
 * data is returned, because, in case of @c AnvVector,
 * the data is the element and both are same and one.
 * In case of trees, each node not only posesses it's own data,
 * but also that of it's children, even if it's indirect.
 * Due to this, removal of node means returning pointer to
 * node and not just the data.
 *
 * @param tree AnvTree* to remove child node from.
 * @param index Position of child node in list of children of given tree.
 *
 * @return Pointer to node after removing the child node.
 * */
 FORCE_INLINE AnvTreeNode* anv_tree_remove(AnvTree* tree, Size index) {
    return anv_tree_node_remove(TO_ANV_TREE_NODE(tree), index);
}

/**
 * Simply delete the node at given index from this tree.
 *
 * @param tree AnvTree* from which child node will be removed.
 * @param index Index of child node in list of childrens.
 * */
FORCE_INLINE void anv_tree_delete(AnvTree* tree, Size index) {
    anv_tree_node_remove(TO_ANV_TREE_NODE(tree), index);
}

#endif // ANV_UTILS_TREE_H

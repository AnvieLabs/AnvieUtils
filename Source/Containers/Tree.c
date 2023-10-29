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

#include <Anvie/Containers/Tree.h>

#include <string.h>

// private functions
static void tree_node_increment_height(TreeNode* node);
static void tree_node_increment_size(TreeNode* node);
static void tree_node_decrement_height(TreeNode* node);
static void tree_node_decrement_size(TreeNode* node);

/**
 * Create a copy of given node object.
 *
 * @param p_dst Pointer to destination memory where
 * new copy will be stored.
 * @param p_src Pointer to tree node to create copy of.
 * */
void tree_node_create_copy(void* p_dst, void* p_src) {
    RETURN_IF_FAIL(p_dst && p_src, ERR_INVALID_ARGUMENTS);

    TreeNode* node_src = TO_TREE_NODE(p_src);
    Tree* tree = node_src->tree_parent;

    // create a copy of given src node to dst node
    TreeNode* node_dst = TO_TREE_NODE(p_dst);
    node_dst->node_parent = node_src->node_parent;
    node_dst->tree_parent = tree;
    node_dst->height = node_src->height;

    // allocate space for storing data (if needed)
    if(tree->pfn_create_copy || (tree->element_size > 8)) {
        node_dst->p_data = ALLOCATE(Uint8, tree->element_size);
        if(!node_dst->p_data) {
            ERR(__FUNCTION__, ERR_OUT_OF_MEMORY);
            return;
        }
    }

    // create copy of data stored in node
    if(tree->pfn_create_copy) {
        tree->pfn_create_copy(node_dst->p_data, node_src->p_data);
    } else if(tree->element_size > 8) {
        memcpy(node_dst->p_data, node_src->p_data, tree->element_size);
    } else {
        node_dst->p_data = node_src->p_data;
    }

    // create clone of all children
    if(node_src->vec_children) {
        node_dst->vec_children = tree_node_vector_clone(node_src->vec_children);
        if(!node_dst->vec_children) {
            ERR(__FUNCTION__, ERR_OUT_OF_MEMORY);
            FREE(node_dst->p_data);
            node_dst->p_data = NULL;
        }
    }
}

/**
 * Destroy the copy of given tree node object.
 *
 * @param p_copy Pointer to copy of TreeNode created
 * using @c tree_node_create_copy().
 * */
void tree_node_destroy_copy(void* p_copy) {
    RETURN_IF_FAIL(p_copy, ERR_INVALID_ARGUMENTS);

    TreeNode* node = TO_TREE_NODE(p_copy);
    Tree* tree = TREE_PARENT(node);

    // if element was created using a create_copy() or it's size is greater than 8
    // then free it
    if(tree->pfn_destroy_copy || tree->element_size > 8) {
        tree->pfn_destroy_copy(node->p_data);
        FREE(node->p_data);
    }
    node->p_data = NULL;

    // TODO: make sure this will destroy the tree recursively!
    if(node->vec_children) {
        tree_node_vector_destroy(node->vec_children);
    }
}

/**
 * Get total number of child objects. This not only includes the
 * immediate child objects but also grand, grand grand, grand grand grand... child
 * objects.
 *
 * @param node
 *
 * @return Total size of given node. Returns 0 if node is NULL
 * without any error message.
 * */
Size tree_node_get_size(TreeNode* node) {
    if(!node) return 0;
    return node->size;
}

/**
 * Get height of given tree node.
 *
 * @param node
 *
 * @return Total size of given node. Returns 0 if node is NULL
 * without any error message.
 * */
Size tree_node_get_height(TreeNode* node) {
    if(!node) return 0;
    return node->height;
}

/**
 * Get first child object.
 *
 * @param node
 *
 * @return Pointer to first child node.
 * */
TreeNode* tree_node_get_front(TreeNode* node) {
    RETURN_VALUE_IF_FAIL(node, NULL, ERR_INVALID_ARGUMENTS);

    if(!node->vec_children || !node->vec_children->length) {
        return NULL;
    }

    return tree_node_vector_front(node->vec_children);
}

/**
 * Get first child object.
 *
 * @param node
 *
 * @return Pointer to last child node.
 * */
TreeNode* tree_node_get_back(TreeNode* node) {
    RETURN_VALUE_IF_FAIL(node, NULL, ERR_INVALID_ARGUMENTS);

    if(!node->vec_children || !node->vec_children->length) {
        return NULL;
    }

    return tree_node_vector_front(node->vec_children);
}

/**
 * Get first child object at given index.
 *
 * @param node
 * @param idx
 *
 * @return Pointer to first child node.
 * */
TreeNode* tree_node_peek(TreeNode* node, Size idx) {
    RETURN_VALUE_IF_FAIL(node, NULL, ERR_INVALID_ARGUMENTS);

    if(!node->vec_children || !node->vec_children->length) {
        return NULL;
    }

    RETURN_VALUE_IF_FAIL(idx < node->vec_children->length, NULL, "Provided index is exceeding the last index in array of child nodes\n")

    return tree_node_vector_peek(node->vec_children, idx);
}

/**
 * Add a new child node and return the added child node.
 *
 * @param node Parent node to add child node to.
 * @param p_data Data for the new added child node.
 *
 * @return Pointer to new added child node on success, NULL otherwise.
 * */
#define TREE_PUSH(place)                                                \
    TreeNode* tree_node_push_##place(TreeNode* node, void* p_data) { \
        RETURN_VALUE_IF_FAIL(node, NULL, ERR_INVALID_ARGUMENTS);        \
                                                                        \
        TreeNode node_child;                                         \
        node_child.node_parent = node;                                  \
        node_child.tree_parent = TREE_PARENT(node);                     \
        node_child.p_data = p_data;                                     \
        node_child.height = 1;                                          \
        node_child.size = 1;                                            \
                                                                        \
        if(!node->vec_children) {                                       \
            node->vec_children = tree_node_vector_create();         \
        }                                                               \
                                                                        \
        if(!node->vec_children->length) {                               \
            tree_node_increment_height(node);                           \
        }                                                               \
        tree_node_increment_size(node);                                 \
                                                                        \
        tree_node_vector_push_##place(node->vec_children, &node_child); \
        return tree_node_vector_##place(node->vec_children);        \
    }

TREE_PUSH(front)
TREE_PUSH(back)
#undef TREE_PUSH

/**
 * Insert a child node to given tree node.
 * This method is slower but is order preserving.
 *
 * @param node Parent tree node.
 * @param p_data Data to be stored in node.
 * @param index Index where node needs to be inserted.
 *
 * @return Pointer to new inserted node.
 * */
TreeNode* tree_node_insert(TreeNode* node, void* p_data, Size index) {
    RETURN_VALUE_IF_FAIL(node, NULL, ERR_INVALID_ARGUMENTS);

    // create temporary child node object
    TreeNode node_child;
    node_child.node_parent = node;
    node_child.tree_parent = TREE_PARENT(node);
    node_child.p_data = p_data;

    // Note that we're not creating copy of data here.
    // This will automatically happen when this node will be inserted
    // in the tree node vector containing children of this node.
    // push_back() -> node_create_copy() -> element_create_copy() -> data copied!

    // create vector and add this child object.
    if(!node->vec_children) {
        node->vec_children = tree_node_vector_create();
    }

    // tree height is changed only when one of it's children's height get
    // updated or when it's first child is added.
    if(!node->vec_children->length) {
        tree_node_increment_height(node);
    }
    // size is incremented at every insertion
    tree_node_increment_size(node);

    tree_node_vector_insert(node->vec_children, &node_child, index);
    return tree_node_vector_peek(node->vec_children, index);
}

/**
 * Remove the child node at given index. This preserves
 * the order of child nodes.
 *
 * @param node
 * @param index
 *
 * @return Pointer to removed child node on success, NULL otherwise.
 * */
TreeNode* tree_node_remove(TreeNode* node, Size index) {
    RETURN_VALUE_IF_FAIL(node, NULL, ERR_INVALID_ARGUMENTS);
    RETURN_VALUE_IF_FAIL(node->vec_children && (index < node->vec_children->length), NULL, "Provided index is exceeding the last index in array of child nodes\n");

    TreeNode* node_child = tree_node_vector_remove(node->vec_children, index);

    tree_node_decrement_size(node);
    if(!node->vec_children->length) {
        tree_node_decrement_height(node);
    }

    return node_child;
}

/**
 * Delete the child node at given index. This preserves
 * the order of child nodes.
 *
 * @param node
 * @param index
 * */
void tree_node_delete(TreeNode* node, Size index) {
    RETURN_IF_FAIL(node, ERR_INVALID_ARGUMENTS);
    RETURN_IF_FAIL(node->vec_children && (index < node->vec_children->length), "Provided index is exceeding the last index in array of child nodes\n");

    tree_node_vector_delete(node->vec_children, index);

    tree_node_decrement_size(node);
    if(!node->vec_children->length) {
        tree_node_decrement_height(node);
    }
}

/**
 * Insert a child node at the very front of given tree node.
 * This method is faster, but is NOT order preserving.
 *
 * @param node Parent tree node.
 * @param p_data Data to be stored in node.
 *
 * @return Pointer to new added node.
 * */
TreeNode* tree_node_push_front_fast(TreeNode* node, void* p_data) {
    RETURN_VALUE_IF_FAIL(node, NULL, ERR_INVALID_ARGUMENTS);

    // create temporary child node object
    TreeNode node_child;
    node_child.node_parent = node;
    node_child.tree_parent = TREE_PARENT(node);
    node_child.p_data = p_data;

    if(!node->vec_children) {
        node->vec_children = tree_node_vector_create();
    }

    if(!node->vec_children->length) {
        tree_node_increment_height(node);
    }
    tree_node_increment_size(node);

    tree_node_vector_push_front_fast(node->vec_children, &node_child);
    return tree_node_vector_front(node->vec_children);
}

/**
 * Insert a child node to given tree node.
 * This method is faster but is NOT order preserving.
 *
 * @param node Parent node to insert child node to.
 * @param p_data Data to be set in new child node.
 * @param index Index where new child node will be inserted.
 *
 * @return Pointer to removed child node on success, NULL otherwise.
 * */
TreeNode* tree_node_insert_fast(TreeNode* node, void* p_data, Size index) {
    RETURN_VALUE_IF_FAIL(node, NULL, ERR_INVALID_ARGUMENTS);

    // create temporary child node object
    TreeNode node_child;
    node_child.node_parent = node;
    node_child.tree_parent = TREE_PARENT(node);
    node_child.p_data = p_data;

    // create child vector if not already created and insert
    if(!node->vec_children) {
        node->vec_children = tree_node_vector_create();
    }

    // update height and size
    if(!node->vec_children->length) {
        tree_node_increment_height(node);
    }
    tree_node_increment_size(node);

    tree_node_vector_insert_fast(node->vec_children, &node_child, index);
    return tree_node_vector_peek(node->vec_children, index);
}

/**
 * Remove the child node at given index. This DOES NOT preserve
 * the order of child nodes. This remove method is faster than
 * normal remove method.
 *
 * @param node Parent node to insert child node to.
 * @param index Index where new child node will be inserted.
 *
 * @return Pointer to removed child node on success, NULL otherwise.
 * */
TreeNode* tree_node_remove_fast(TreeNode* node, Size index) {
    RETURN_VALUE_IF_FAIL(node, NULL, ERR_INVALID_ARGUMENTS);
    RETURN_VALUE_IF_FAIL(node->vec_children && (index < node->vec_children->length), NULL, "Provided index is exceeding the last index in array of child nodes\n");

    TreeNode* node_child = tree_node_vector_remove_fast(node->vec_children, index);

    tree_node_decrement_size(node);
    if(!node->vec_children->length) {
        tree_node_increment_height(node);
    }

    return node_child;
}

/**
 * Delete the child node at given index. This DOES NOT preserve
 * the order of child nodes. This delete method is faster than
 * normal delete method.
 *
 * @param node Parent node to insert child node to.
 * @param index Index where new child node will be inserted.
 * */
void tree_node_delete_fast(TreeNode* node, Size index) {
    RETURN_IF_FAIL(node, ERR_INVALID_ARGUMENTS);
    RETURN_IF_FAIL(node->vec_children && (index < node->vec_children->length), "Provided index is exceeding the last index in array of child nodes\n");

    tree_node_vector_delete_fast(node->vec_children, index);

    tree_node_decrement_size(node);
    if(!node->vec_children->length) {
        tree_node_increment_height(node);
    }
}

/************************** PRIVATE FUNCTIONS ***************************/

#define TREE_NODE_INCREMENT_PROP(prop)                    \
    static void tree_node_increment_##prop(TreeNode* node) { \
        if(!node) return;                                               \
        node->prop++;                                                   \
        if(node->node_parent) tree_node_increment_##prop(node->node_parent); \
    }
TREE_NODE_INCREMENT_PROP(height)
TREE_NODE_INCREMENT_PROP(size)
#undef TREE_NODE_INCREMENT_PROP

#define TREE_NODE_DECREMENT_PROP(prop)                    \
    static void tree_node_decrement_##prop(TreeNode* node) { \
        if(!node) return;                                               \
        node->prop--;                                                   \
        if(node->node_parent) tree_node_decrement_##prop(node->node_parent); \
    }
TREE_NODE_DECREMENT_PROP(height)
TREE_NODE_DECREMENT_PROP(size)
#undef TREE_NODE_DECREMENT_PROP

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
static void tree_node_increment_height(TreeNode* node); /* used as a recursive operation */
static void tree_node_increment_size(TreeNode* node); /* used as a recursive operation */
static void tree_node_decrement_height(TreeNode* node); /* used as a recursive operation */
static void tree_node_decrement_size(TreeNode* node); /* used as a recursive operation */

/**
 * Create a copy of given node object.
 * @param dst Pointer to destination memory where
 * new copy will be stored.
 * @param src Pointer to tree node to create copy of.
 * @param udata User data passed to callback functions.
 * This data will be passed recursively to copy constructor
 * of create copy of every @c TreeNode in the provided src @c TreeNode.
 * */
void tree_node_create_copy(void* dst, void* src, void* udata) {
    RETURN_IF_FAIL(dst && src, ERR_INVALID_ARGUMENTS);

    TreeNode* node_src = TO_TREE_NODE(src);
    Tree* tree = node_src->tree_parent;

    // create a copy of given src node to dst node
    TreeNode* node_dst = TO_TREE_NODE(dst);
    node_dst->node_parent = node_src->node_parent;
    node_dst->tree_parent = tree;
    node_dst->height      = node_src->height;

    // allocate space for storing data (if needed)
    if(tree->create_copy || (tree->element_size > 8)) {
        node_dst->data = ALLOCATE(Uint8, tree->element_size);
        if(!node_dst->data) {
            ERR(__FUNCTION__, ERR_OUT_OF_MEMORY);
            return;
        }
    }

    // create copy of data stored in node
    if(tree->create_copy) {
        tree->create_copy(node_dst->data, node_src->data, udata);
    } else if(tree->element_size > 8) {
        memcpy(node_dst->data, node_src->data, tree->element_size);
    } else {
        node_dst->data = node_src->data;
    }

    // create clone of all children
    if(node_src->children) {
        node_dst->children = tree_node_vector_clone(node_src->children, udata);
        if(!node_dst->children) {
            ERR(__FUNCTION__, "Failed to create Vector<TreeNode> clone\n");
            FREE(node_dst->data);
            node_dst->data = NULL;
        }
    }
}

/**
 * Destroy the copy of given tree node object.
 * @param copy Pointer to copy of TreeNode created
 * @param udata User data passed to callback functions.
 * This data will be passed recursively to copy destructor
 * of destroy every @c TreeNode in the provided @c TreeNode.
 * */
void tree_node_destroy_copy(void* copy, void* udata) {
    RETURN_IF_FAIL(copy, ERR_INVALID_ARGUMENTS);

    TreeNode* node = TO_TREE_NODE(copy);
    Tree* tree = TREE_PARENT(node);

    // if element was created using a create_copy() or it's size is greater than 8
    // then free it
    if(tree->destroy_copy || tree->element_size > 8) {
        tree->destroy_copy(node->data, udata);
        FREE(node->data);
    }
    node->data = NULL;

    // TODO: make sure this will destroy the tree recursively!
    if(node->children) {
        tree_node_vector_destroy(node->children, udata);
    }
}

/**
 * Get total number of child objects. This not only includes the
 * immediate child objects but also grand, grand grand, grand grand grand... child
 * objects.
 * @param node
 * @return Total size of given node. Returns 0 if node is NULL
 * without any error message.
 * */
Size tree_node_size(TreeNode* node) {
    if(!node) return 0;
    return node->size;
}

/**
 * Get height of given tree node.
 * @param node
 * @return Total size of given node. Returns 0 if node is NULL
 * without any error message.
 * */
Size tree_node_height(TreeNode* node) {
    if(!node) return 0;
    return node->height;
}

/**
 * Get first child object.
 * @param node
 * @return Pointer to first child node.
 * */
TreeNode* tree_node_front(TreeNode* node) {
    RETURN_VALUE_IF_FAIL(node, NULL, ERR_INVALID_ARGUMENTS);
    if(!node->children || !node->children->length) {
        return NULL;
    }
    return tree_node_vector_front(node->children);
}

/**
 * Get first child object.
 * @param node
 * @return Pointer to last child node.
 * */
TreeNode* tree_node_back(TreeNode* node) {
    RETURN_VALUE_IF_FAIL(node, NULL, ERR_INVALID_ARGUMENTS);

    if(!node->children || !node->children->length) {
        return NULL;
    }

    return tree_node_vector_front(node->children);
}

/**
 * Get first child object at given index.
 * @param node
 * @param idx
 * @return Pointer to first child node.
 * */
TreeNode* tree_node_peek(TreeNode* node, Size idx) {
    RETURN_VALUE_IF_FAIL(node, NULL, ERR_INVALID_ARGUMENTS);

    if(!node->children || !node->children->length) {
        return NULL;
    }
    RETURN_VALUE_IF_FAIL(idx < node->children->length, NULL, "Provided index is exceeding the last index in array of child nodes\n")

    return tree_node_vector_peek(node->children, idx);
}

/**
 * Add a new child node and return the added child node.
 * @param node Parent node to add child node to.
 * @param data Data for the new added child node.
 * @param udata User data passed to callback functions.
 * This data will be passed to provided copy constructor of @c TreeNode
 * to create a copy of provided data for this @c TreeNode.
 * @return TreeNode object added to given node on success, NULL otherwise.
 * */
#define TREE_PUSH(place)                                                \
    TreeNode* tree_node_push_##place(TreeNode* node, void* data, void* udata) { \
        RETURN_VALUE_IF_FAIL(node, NULL, ERR_INVALID_ARGUMENTS);        \
                                                                        \
        TreeNode node_child;                                            \
        node_child.node_parent = node;                                  \
        node_child.tree_parent = TREE_PARENT(node);                     \
        node_child.data        = data;                                  \
        node_child.height      = 1;                                     \
        node_child.size        = 1;                                     \
                                                                        \
        if(!node->children) {                                           \
            node->children = tree_node_vector_create();                 \
        }                                                               \
                                                                        \
        if(!node->children->length) {                                   \
            tree_node_increment_height(node);                           \
        }                                                               \
        tree_node_increment_size(node);                                 \
                                                                        \
        tree_node_vector_push_##place(node->children, &node_child, udata); \
        return tree_node_vector_##place(node->children);                \
    }

TREE_PUSH(front)
TREE_PUSH(back)
#undef TREE_PUSH

/**
 * Insert a child node to given tree node.
 * This method is slower but is order preserving.
 * @param node Parent tree node.
 * @param data Data to be stored in node.
 * @param index Index where node needs to be inserted.
 * @param udata User data passed to callback functions.
 * This data will be passed to copy constructor
 * to create a copy of provided data for this @c TreeNode.
 * @return Pointer to new inserted node.
 * */
TreeNode* tree_node_insert(TreeNode* node, void* data, Size index, void* udata) {
    RETURN_VALUE_IF_FAIL(node, NULL, ERR_INVALID_ARGUMENTS);

    // create temporary child node object
    TreeNode node_child;
    node_child.node_parent = node; /* provided node is now an immediate parent of new node */
    node_child.tree_parent = TREE_PARENT(node); /* get tree parent from given node */
    node_child.data        = data; /* set data of this new node */

    // Note that we're not creating copy of data here.
    // This will automatically happen when this node will be inserted
    // in the tree node vector containing children of this node.
    // push_back() -> node_create_copy() -> element_create_copy() -> data copied!

    // create vector and add this child object.
    if(!node->children) {
        node->children = tree_node_vector_create();
    }

    // tree height is changed only when one of it's children's height get updated
    // or when it's first child is added.
    if(!node->children->length) {
        tree_node_increment_height(node);
    }

    // size is incremented at every insertion
    tree_node_increment_size(node);

    tree_node_vector_insert(node->children, &node_child, index, udata);
    return tree_node_vector_peek(node->children, index);
}

/**
 * Remove the child node at given index. This preserves
 * the order of child nodes.
 * @param node
 * @param index
 * @return Pointer to removed child node on success, NULL otherwise.
 * */
TreeNode* tree_node_remove(TreeNode* node, Size index) {
    RETURN_VALUE_IF_FAIL(node, NULL, ERR_INVALID_ARGUMENTS);
    RETURN_VALUE_IF_FAIL(node->children && (index < node->children->length), NULL, "Provided index is exceeding the last index in array of child nodes\n");

    TreeNode* node_child = tree_node_vector_remove(node->children, index);

    // tree size is updated on every removal
    tree_node_decrement_size(node);

    // tree height is updated only when number of children reach 0
    if(!node->children->length) {
        tree_node_decrement_height(node);
    }

    return node_child;
}

/**
 * Delete the child node at given index. This preserves
 * the order of child nodes.
 * @param node
 * @param index
 * @param udata User data passed to callback functions.
 * This data will be passed tocopy destructor
 * to destroy this @c TreeNode's copy of data.
 * */
void tree_node_delete(TreeNode* node, Size index, void* udata) {
    RETURN_IF_FAIL(node, ERR_INVALID_ARGUMENTS);
    RETURN_IF_FAIL(node->children && (index < node->children->length), "Provided index is exceeding the last index in array of child nodes\n");

    tree_node_vector_delete(node->children, index, udata);

    // tree node size is updated every deletion
    tree_node_decrement_size(node);

    // tree node height is updated when number of children reach 0
    if(!node->children->length) {
        tree_node_decrement_height(node);
    }
}

/**
 * Insert a child node at the very front of given tree node.
 * This method is faster, but is NOT order preserving.
 * @param node Parent tree node.
 * @param data Data to be stored in node.
 * @param udata User data passed to callback functions.
 * This data will be passed to copy constructor
 * to create this @c TreeNode's copy of data.
 * @return Pointer to new added node.
 * */
TreeNode* tree_node_push_front_fast(TreeNode* node, void* data, void* udata) {
    RETURN_VALUE_IF_FAIL(node, NULL, ERR_INVALID_ARGUMENTS);

    // create temporary child node object
    TreeNode node_child;
    node_child.node_parent = node;
    node_child.tree_parent = TREE_PARENT(node);
    node_child.data        = data;

    if(!node->children) {
        node->children = tree_node_vector_create();
    }

    if(!node->children->length) {
        tree_node_increment_height(node);
    }
    tree_node_increment_size(node);

    tree_node_vector_push_front_fast(node->children, &node_child, udata);
    return tree_node_vector_front(node->children);
}

/**
 * Insert a child node to given tree node.
 * This method is faster but is NOT order preserving.
 * @param node Parent node to insert child node to.
 * @param data Data to be set in new child node.
 * @param index Index where new child node will be inserted.
 * @param udata User data passed to callback functions.
 * This data will be passed to copy constructor
 * to create this @c TreeNode's copy of data.
 * @return Pointer to removed child node on success, NULL otherwise.
 * */
TreeNode* tree_node_insert_fast(TreeNode* node, void* data, Size index, void* udata) {
    RETURN_VALUE_IF_FAIL(node, NULL, ERR_INVALID_ARGUMENTS);

    // create temporary child node object
    TreeNode node_child;
    node_child.node_parent = node;
    node_child.tree_parent = TREE_PARENT(node);
    node_child.data        = data;

    // create child vector if not already created and insert
    if(!node->children) {
        node->children = tree_node_vector_create();
    }

    // update height and size
    if(!node->children->length) {
        tree_node_increment_height(node);
    }
    tree_node_increment_size(node);

    tree_node_vector_insert_fast(node->children, &node_child, index, udata);
    return tree_node_vector_peek(node->children, index);
}

/**
 * Remove the child node at given index. This DOES NOT preserve
 * the order of child nodes. This remove method is faster than
 * normal remove method.
 * @param node Parent node to insert child node to.
 * @param index Index where new child node will be inserted.
 * @return Pointer to removed child node on success, NULL otherwise.
 * */
TreeNode* tree_node_remove_fast(TreeNode* node, Size index) {
    RETURN_VALUE_IF_FAIL(node, NULL, ERR_INVALID_ARGUMENTS);
    RETURN_VALUE_IF_FAIL(node->children && (index < node->children->length), NULL, "Provided index is exceeding the last index in array of child nodes\n");

    TreeNode* node_child = tree_node_vector_remove_fast(node->children, index);

    tree_node_decrement_size(node);
    if(!node->children->length) {
        tree_node_increment_height(node);
    }

    return node_child;
}

/**
 * Delete the child node at given index. This DOES NOT preserve
 * the order of child nodes. This delete method is faster than
 * normal delete method.
 * @param node Parent node to insert child node to.
 * @param index Index where new child node will be inserted.
 * @param udata User data passed to callback functions.
 * This data will be passed to copy constructor
 * to create this @c TreeNode's copy of data.
 * */
void tree_node_delete_fast(TreeNode* node, Size index, void* udata) {
    RETURN_IF_FAIL(node, ERR_INVALID_ARGUMENTS);
    RETURN_IF_FAIL(node->children && (index < node->children->length), "Provided index is exceeding the last index in array of child nodes\n");

    tree_node_vector_delete_fast(node->children, index, udata);

    tree_node_decrement_size(node);
    if(!node->children->length) {
        tree_node_increment_height(node);
    }
}

/************************** PRIVATE FUNCTIONS ***************************/

#define TREE_NODE_INCREMENT_PROP(prop)                                  \
    static void tree_node_increment_##prop(TreeNode* node) {            \
        if(!node) return;                                               \
        node->prop++;                                                   \
        if(node->node_parent) tree_node_increment_##prop(node->node_parent); \
    }
TREE_NODE_INCREMENT_PROP(height)
TREE_NODE_INCREMENT_PROP(size)
#undef TREE_NODE_INCREMENT_PROP

#define TREE_NODE_DECREMENT_PROP(prop)                                  \
    static void tree_node_decrement_##prop(TreeNode* node) {            \
        if(!node) return;                                               \
        node->prop--;                                                   \
        if(node->node_parent) tree_node_decrement_##prop(node->node_parent); \
    }
TREE_NODE_DECREMENT_PROP(height)
TREE_NODE_DECREMENT_PROP(size)
#undef TREE_NODE_DECREMENT_PROP

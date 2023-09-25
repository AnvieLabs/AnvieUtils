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

/**
 * Create a copy of given node object.
 *
 * @param p_dst Pointer to destination memory where
 * new copy will be stored.
 * @param p_src Pointer to tree node to create copy of.
 * */
void anv_tree_node_create_copy(void* p_dst, void* p_src) {
    RETURN_IF_FAIL(p_dst && p_src, ERR_INVALID_ARGUMENTS);

    AnvTreeNode* node_dst = TO_ANV_TREE_NODE(p_dst);
    AnvTreeNode* node_src = TO_ANV_TREE_NODE(p_src);

    node_dst->node_parent = node_src->node_parent;
    node_dst->tree_parent = node_src->tree_parent;

    // get tree and allocate space for data
    AnvTree* tree = TO_ANV_TREE(node_dst->tree_parent);
    node_dst->p_data = ALLOCATE(Uint8, tree->element_size);
    RETURN_IF_FAIL(node_dst->p_data, ERR_OUT_OF_MEMORY);

    // create a fresh, new copy of data
    if(tree->pfn_create_copy) {
        tree->pfn_create_copy(node_dst->p_data, node_src->p_data);
    } else {
        memcpy(node_dst->p_data, node_src->p_data, tree->element_size);
    }

    // create clone of all children
    if(node_src->vec_children) {
        node_dst->vec_children = anv_tree_node_vector_clone(node_src->vec_children);
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
 * @param p_copy Pointer to copy of AnvTreeNode created
 * using @c anv_tree_node_create_copy().
 * */
void anv_tree_node_destroy_copy(void* p_copy) {
    RETURN_IF_FAIL(p_copy, ERR_INVALID_ARGUMENTS);

    AnvTreeNode* node = TO_ANV_TREE_NODE(p_copy);
    AnvTree* tree = TO_ANV_TREE(node->tree_parent);

    if(tree->pfn_destroy_copy) {
        tree->pfn_destroy_copy(node->p_data);
    }
    FREE(node->p_data);

    // TODO: make sure this will destroy the tree recursively!
    if(node->vec_children) {
        anv_tree_node_vector_destroy(node->vec_children);
    }
}

Size anv_tree_node_get_size(AnvTreeNode* node) {
    UNUSED(node);
    return 0;
}

Size anv_tree_node_get_height(AnvTreeNode* node) {
    UNUSED(node);
    return 0;
}

// TODO: do we want to return this node or child node here?
AnvTreeNode* anv_tree_node_add(AnvTreeNode* node, void* p_data) {
    UNUSED(node && p_data);
    return NULL;
}

// TODO: same goes for here!
AnvTreeNode* anv_tree_node_remove(AnvTreeNode* node, Size index) {
    UNUSED(node && index);
    return NULL;
}

void anv_tree_node_delete(AnvTreeNode* node, Size index) {
    UNUSED(node && index);
}

/**
 * @file TreeInterface.h
 * @date Mon, 14th July, 2023
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
 * @brief Defines macros that'll help in quick creation of trees for any type.
 * Take a look at IntegerTreeTests.c and StructTreeTests.c for more insight on how to use these.
 * */

#ifndef UTILS_INTERFACE_TREE_H
#define UTILS_INTERFACE_TREE_H

#define DEF_INTEGER_TREE_INTERFACE(prefix, type) DEF_INTEGER_TREE_INTERFACE_WITH_COPY_AND_DESTROY(prefix, type, NULL, NULL)

#define DEF_INTEGER_TREE_INTERFACE_WITH_COPY_AND_DESTROY(prefix, type, copy, destroy) \
    FORCE_INLINE Tree* prefix##_tree_create() {                \
        return tree_create(sizeof(type), copy, destroy);            \
    }                                                                   \
                                                                        \
    FORCE_INLINE Size prefix##_tree_get_size(Tree* node) { \
        return tree_get_size(node);                                 \
    }                                                                   \
                                                                        \
    FORCE_INLINE Size prefix##_tree_get_height(Tree* node) { \
        return tree_get_height(node);                               \
    }                                                                   \
                                                                        \
    FORCE_INLINE TreeNode* prefix##_tree_push_front(Tree* node, type data) { \
        return tree_push_front(node, (void*)(Uint64)data);        \
    }                                                                   \
                                                                        \
    FORCE_INLINE TreeNode* prefix##_tree_push_back(Tree* node, type data) { \
        return tree_push_back(node, (void*)(Uint64)data);                 \
    }                                                                   \
                                                                        \
    FORCE_INLINE TreeNode* prefix##_tree_insert(Tree* node, type data, Size index) { \
        return tree_insert(node, (void*)(Uint64)data, index);             \
    }                                                                   \
                                                                        \
    FORCE_INLINE TreeNode* prefix##_tree_remove(Tree* node, Size index) { \
        return tree_remove(node, index);                            \
    }                                                                   \
                                                                        \
    FORCE_INLINE void prefix##_tree_delete(Tree* node, Size index) { \
        tree_delete(node, index);                                   \
    }                                                                   \
                                                                        \
    FORCE_INLINE TreeNode* prefix##_tree_push_front_fast(Tree* node, type data) { \
        return tree_push_front_fast(node, (void*)(Uint64)data);           \
    }                                                                   \
                                                                        \
    FORCE_INLINE TreeNode* prefix##_tree_insert_fast(Tree* node, type data, Size index) { \
        return tree_insert_fast(node, (void*)(Uint64)data, index);        \
    }                                                                   \
                                                                        \
    FORCE_INLINE TreeNode* prefix##_tree_remove_fast(Tree* node, Size index) { \
        return tree_remove_fast(node, index);                       \
    }                                                                   \
                                                                        \
    FORCE_INLINE void prefix##_tree_delete_fast(Tree* node, Size index) { \
        tree_delete_fast(node, index);                              \
    }                                                                   \
                                                                        \
    FORCE_INLINE Size prefix##_tree_node_get_size(TreeNode* node) { \
        return tree_node_get_size(node);                            \
    }                                                                   \
                                                                        \
    FORCE_INLINE Size prefix##_tree_node_get_height(TreeNode* node) { \
        return tree_node_get_height(node);                          \
    }                                                                   \
                                                                        \
    FORCE_INLINE TreeNode* prefix##_tree_node_push_front(TreeNode* node, type data) { \
        return tree_node_push_front(node, (void*)(Uint64)data);           \
    }                                                                   \
                                                                        \
    FORCE_INLINE TreeNode* prefix##_tree_node_push_back(TreeNode* node, type data) { \
        return tree_node_push_back(node, (void*)(Uint64)data);            \
    }                                                                   \
                                                                        \
    FORCE_INLINE TreeNode* prefix##_tree_node_insert(TreeNode* node, type data, Size index) { \
        return tree_node_insert(node, (void*)(Uint64)data, index);        \
    }                                                                   \
                                                                        \
    FORCE_INLINE TreeNode* prefix##_tree_node_remove(TreeNode* node, Size index) { \
        return tree_node_remove(node, index);                       \
    }                                                                   \
                                                                        \
    FORCE_INLINE void prefix##_tree_node_delete(TreeNode* node, Size index) { \
        tree_node_delete(node, index);                              \
    }                                                                   \
                                                                        \
    FORCE_INLINE TreeNode* prefix##_tree_node_push_front_fast(TreeNode* node, type data) { \
        return tree_node_push_front_fast(node, (void*)(Uint64)data);      \
    }                                                                   \
                                                                        \
    FORCE_INLINE TreeNode* prefix##_tree_node_insert_fast(TreeNode* node, type data, Size index) { \
        return tree_node_insert_fast(node, (void*)(Uint64)data, index);   \
    }                                                                   \
                                                                        \
    FORCE_INLINE TreeNode* prefix##_tree_node_remove_fast(TreeNode* node, Size index) { \
        return tree_node_remove_fast(node, index);                  \
    }                                                                   \
                                                                        \
    FORCE_INLINE void prefix##_tree_node_delete_fast(TreeNode* node, Size index) { \
        tree_node_delete_fast(node, index);                         \
    }

#define DEF_STRUCT_TREE_INTERFACE(prefix, type, copy, destroy)      \
    FORCE_INLINE Tree* prefix##_tree_create() {                \
        return tree_create(sizeof(type), copy, destroy);            \
    }                                                                   \
                                                                        \
    FORCE_INLINE Size prefix##_tree_get_size(Tree* node) { \
        return tree_get_size(node);                                 \
    }                                                                   \
                                                                        \
    FORCE_INLINE Size prefix##_tree_get_height(Tree* node) { \
        return tree_get_height(node);                               \
    }                                                                   \
                                                                        \
    FORCE_INLINE TreeNode* prefix##_tree_push_front(Tree* node, type* data) { \
        return tree_push_front(node, (void*)data);                \
    }                                                                   \
                                                                        \
    FORCE_INLINE TreeNode* prefix##_tree_push_back(Tree* node, type* data) { \
        return tree_push_back(node, (void*)data);                 \
    }                                                                   \
                                                                        \
    FORCE_INLINE TreeNode* prefix##_tree_insert(Tree* node, type* data, Size index) { \
        return tree_insert(node, (void*)data, index);             \
    }                                                                   \
                                                                        \
    FORCE_INLINE TreeNode* prefix##_tree_remove(Tree* node, Size index) { \
        return tree_remove(node, index);                            \
    }                                                                   \
                                                                        \
    FORCE_INLINE void prefix##_tree_delete(Tree* node, Size index) { \
        tree_delete(node, index);                                   \
    }                                                                   \
                                                                        \
    FORCE_INLINE TreeNode* prefix##_tree_push_front_fast(Tree* node, type* data) { \
        return tree_push_front_fast(node, (void*)data);           \
    }                                                                   \
                                                                        \
    FORCE_INLINE TreeNode* prefix##_tree_insert_fast(Tree* node, type* data, Size index) { \
        return tree_insert_fast(node, (void*)data, index);        \
    }                                                                   \
                                                                        \
    FORCE_INLINE TreeNode* prefix##_tree_remove_fast(Tree* node, Size index) { \
        return tree_remove_fast(node, index);                       \
    }                                                                   \
                                                                        \
    FORCE_INLINE void prefix##_tree_delete_fast(Tree* node, Size index) { \
        tree_delete_fast(node, index);                              \
    }                                                                   \
                                                                        \
    FORCE_INLINE Size prefix##_tree_node_get_size(TreeNode* node) { \
        return tree_node_get_size(node);                            \
    }                                                                   \
                                                                        \
    FORCE_INLINE Size prefix##_tree_node_get_height(TreeNode* node) { \
        return tree_node_get_height(node);                          \
    }                                                                   \
                                                                        \
    FORCE_INLINE TreeNode* prefix##_tree_node_push_front(TreeNode* node, type* data) { \
        return tree_node_push_front(node, (void*)data);           \
    }                                                                   \
                                                                        \
    FORCE_INLINE TreeNode* prefix##_tree_node_push_back(TreeNode* node, type* data) { \
        return tree_node_push_back(node, (void*)data);            \
    }                                                                   \
                                                                        \
    FORCE_INLINE TreeNode* prefix##_tree_node_insert(TreeNode* node, type* data, Size index) { \
        return tree_node_insert(node, (void*)data, index);        \
    }                                                                   \
                                                                        \
    FORCE_INLINE TreeNode* prefix##_tree_node_remove(TreeNode* node, Size index) { \
        return tree_node_remove(node, index);                       \
    }                                                                   \
                                                                        \
    FORCE_INLINE void prefix##_tree_node_delete(TreeNode* node, Size index) { \
        tree_node_delete(node, index);                              \
    }                                                                   \
                                                                        \
    FORCE_INLINE TreeNode* prefix##_tree_node_push_front_fast(TreeNode* node, type* data) { \
        return tree_node_push_front_fast(node, (void*)data);      \
    }                                                                   \
                                                                        \
    FORCE_INLINE TreeNode* prefix##_tree_node_insert_fast(TreeNode* node, type* data, Size index) { \
        return tree_node_insert_fast(node, (void*)data, index);   \
    }                                                                   \
                                                                        \
    FORCE_INLINE TreeNode* prefix##_tree_node_remove_fast(TreeNode* node, Size index) { \
        return tree_node_remove_fast(node, index);                  \
    }                                                                   \
                                                                        \
    FORCE_INLINE void prefix##_tree_node_delete_fast(TreeNode* node, Size index) { \
        tree_node_delete_fast(node, index);                         \
    }


#endif //  UTILS_INTERFACE_TREE_H

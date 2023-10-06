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

#ifndef ANV_UTILS_INTERFACE_TREE_H
#define ANV_UTILS_INTERFACE_TREE_H

#define DEF_ANV_INTEGER_TREE_INTERFACE(prefix, type) DEF_ANV_INTEGER_TREE_INTERFACE_WITH_COPY_AND_DESTROY(prefix, type, NULL, NULL)

#define DEF_ANV_INTEGER_TREE_INTERFACE_WITH_COPY_AND_DESTROY(prefix, type, copy, destroy) \
    FORCE_INLINE AnvTree* anv_##prefix##_tree_create() {                \
        return anv_tree_create(sizeof(type), copy, destroy);            \
    }                                                                   \
                                                                        \
    FORCE_INLINE Size anv_##prefix##_tree_get_size(AnvTree* node) { \
        return anv_tree_get_size(node);                                 \
    }                                                                   \
                                                                        \
    FORCE_INLINE Size anv_##prefix##_tree_get_height(AnvTree* node) { \
        return anv_tree_get_height(node);                               \
    }                                                                   \
                                                                        \
    FORCE_INLINE AnvTreeNode* anv_##prefix##_tree_push_front(AnvTree* node, type p_data) { \
        return anv_tree_push_front(node, (void*)(Uint64)p_data);        \
    }                                                                   \
                                                                        \
    FORCE_INLINE AnvTreeNode* anv_##prefix##_tree_push_back(AnvTree* node, type p_data) { \
        return anv_tree_push_back(node, (void*)(Uint64)p_data);                 \
    }                                                                   \
                                                                        \
    FORCE_INLINE AnvTreeNode* anv_##prefix##_tree_insert(AnvTree* node, type p_data, Size index) { \
        return anv_tree_insert(node, (void*)(Uint64)p_data, index);             \
    }                                                                   \
                                                                        \
    FORCE_INLINE AnvTreeNode* anv_##prefix##_tree_remove(AnvTree* node, Size index) { \
        return anv_tree_remove(node, index);                            \
    }                                                                   \
                                                                        \
    FORCE_INLINE void anv_##prefix##_tree_delete(AnvTree* node, Size index) { \
        anv_tree_delete(node, index);                                   \
    }                                                                   \
                                                                        \
    FORCE_INLINE AnvTreeNode* anv_##prefix##_tree_push_front_fast(AnvTree* node, type p_data) { \
        return anv_tree_push_front_fast(node, (void*)(Uint64)p_data);           \
    }                                                                   \
                                                                        \
    FORCE_INLINE AnvTreeNode* anv_##prefix##_tree_insert_fast(AnvTree* node, type p_data, Size index) { \
        return anv_tree_insert_fast(node, (void*)(Uint64)p_data, index);        \
    }                                                                   \
                                                                        \
    FORCE_INLINE AnvTreeNode* anv_##prefix##_tree_remove_fast(AnvTree* node, Size index) { \
        return anv_tree_remove_fast(node, index);                       \
    }                                                                   \
                                                                        \
    FORCE_INLINE void anv_##prefix##_tree_delete_fast(AnvTree* node, Size index) { \
        anv_tree_delete_fast(node, index);                              \
    }                                                                   \
                                                                        \
    FORCE_INLINE Size anv_##prefix##_tree_node_get_size(AnvTreeNode* node) { \
        return anv_tree_node_get_size(node);                            \
    }                                                                   \
                                                                        \
    FORCE_INLINE Size anv_##prefix##_tree_node_get_height(AnvTreeNode* node) { \
        return anv_tree_node_get_height(node);                          \
    }                                                                   \
                                                                        \
    FORCE_INLINE AnvTreeNode* anv_##prefix##_tree_node_push_front(AnvTreeNode* node, type p_data) { \
        return anv_tree_node_push_front(node, (void*)(Uint64)p_data);           \
    }                                                                   \
                                                                        \
    FORCE_INLINE AnvTreeNode* anv_##prefix##_tree_node_push_back(AnvTreeNode* node, type p_data) { \
        return anv_tree_node_push_back(node, (void*)(Uint64)p_data);            \
    }                                                                   \
                                                                        \
    FORCE_INLINE AnvTreeNode* anv_##prefix##_tree_node_insert(AnvTreeNode* node, type p_data, Size index) { \
        return anv_tree_node_insert(node, (void*)(Uint64)p_data, index);        \
    }                                                                   \
                                                                        \
    FORCE_INLINE AnvTreeNode* anv_##prefix##_tree_node_remove(AnvTreeNode* node, Size index) { \
        return anv_tree_node_remove(node, index);                       \
    }                                                                   \
                                                                        \
    FORCE_INLINE void anv_##prefix##_tree_node_delete(AnvTreeNode* node, Size index) { \
        anv_tree_node_delete(node, index);                              \
    }                                                                   \
                                                                        \
    FORCE_INLINE AnvTreeNode* anv_##prefix##_tree_node_push_front_fast(AnvTreeNode* node, type p_data) { \
        return anv_tree_node_push_front_fast(node, (void*)(Uint64)p_data);      \
    }                                                                   \
                                                                        \
    FORCE_INLINE AnvTreeNode* anv_##prefix##_tree_node_insert_fast(AnvTreeNode* node, type p_data, Size index) { \
        return anv_tree_node_insert_fast(node, (void*)(Uint64)p_data, index);   \
    }                                                                   \
                                                                        \
    FORCE_INLINE AnvTreeNode* anv_##prefix##_tree_node_remove_fast(AnvTreeNode* node, Size index) { \
        return anv_tree_node_remove_fast(node, index);                  \
    }                                                                   \
                                                                        \
    FORCE_INLINE void anv_##prefix##_tree_node_delete_fast(AnvTreeNode* node, Size index) { \
        anv_tree_node_delete_fast(node, index);                         \
    }

#define DEF_ANV_STRUCT_TREE_INTERFACE(prefix, type, copy, destroy)      \
    FORCE_INLINE AnvTree* anv_##prefix##_tree_create() {                \
        return anv_tree_create(sizeof(type), copy, destroy);            \
    }                                                                   \
                                                                        \
    FORCE_INLINE Size anv_##prefix##_tree_get_size(AnvTree* node) { \
        return anv_tree_get_size(node);                                 \
    }                                                                   \
                                                                        \
    FORCE_INLINE Size anv_##prefix##_tree_get_height(AnvTree* node) { \
        return anv_tree_get_height(node);                               \
    }                                                                   \
                                                                        \
    FORCE_INLINE AnvTreeNode* anv_##prefix##_tree_push_front(AnvTree* node, type* p_data) { \
        return anv_tree_push_front(node, (void*)p_data);                \
    }                                                                   \
                                                                        \
    FORCE_INLINE AnvTreeNode* anv_##prefix##_tree_push_back(AnvTree* node, type* p_data) { \
        return anv_tree_push_back(node, (void*)p_data);                 \
    }                                                                   \
                                                                        \
    FORCE_INLINE AnvTreeNode* anv_##prefix##_tree_insert(AnvTree* node, type* p_data, Size index) { \
        return anv_tree_insert(node, (void*)p_data, index);             \
    }                                                                   \
                                                                        \
    FORCE_INLINE AnvTreeNode* anv_##prefix##_tree_remove(AnvTree* node, Size index) { \
        return anv_tree_remove(node, index);                            \
    }                                                                   \
                                                                        \
    FORCE_INLINE void anv_##prefix##_tree_delete(AnvTree* node, Size index) { \
        anv_tree_delete(node, index);                                   \
    }                                                                   \
                                                                        \
    FORCE_INLINE AnvTreeNode* anv_##prefix##_tree_push_front_fast(AnvTree* node, type* p_data) { \
        return anv_tree_push_front_fast(node, (void*)p_data);           \
    }                                                                   \
                                                                        \
    FORCE_INLINE AnvTreeNode* anv_##prefix##_tree_insert_fast(AnvTree* node, type* p_data, Size index) { \
        return anv_tree_insert_fast(node, (void*)p_data, index);        \
    }                                                                   \
                                                                        \
    FORCE_INLINE AnvTreeNode* anv_##prefix##_tree_remove_fast(AnvTree* node, Size index) { \
        return anv_tree_remove_fast(node, index);                       \
    }                                                                   \
                                                                        \
    FORCE_INLINE void anv_##prefix##_tree_delete_fast(AnvTree* node, Size index) { \
        anv_tree_delete_fast(node, index);                              \
    }                                                                   \
                                                                        \
    FORCE_INLINE Size anv_##prefix##_tree_node_get_size(AnvTreeNode* node) { \
        return anv_tree_node_get_size(node);                            \
    }                                                                   \
                                                                        \
    FORCE_INLINE Size anv_##prefix##_tree_node_get_height(AnvTreeNode* node) { \
        return anv_tree_node_get_height(node);                          \
    }                                                                   \
                                                                        \
    FORCE_INLINE AnvTreeNode* anv_##prefix##_tree_node_push_front(AnvTreeNode* node, type* p_data) { \
        return anv_tree_node_push_front(node, (void*)p_data);           \
    }                                                                   \
                                                                        \
    FORCE_INLINE AnvTreeNode* anv_##prefix##_tree_node_push_back(AnvTreeNode* node, type* p_data) { \
        return anv_tree_node_push_back(node, (void*)p_data);            \
    }                                                                   \
                                                                        \
    FORCE_INLINE AnvTreeNode* anv_##prefix##_tree_node_insert(AnvTreeNode* node, type* p_data, Size index) { \
        return anv_tree_node_insert(node, (void*)p_data, index);        \
    }                                                                   \
                                                                        \
    FORCE_INLINE AnvTreeNode* anv_##prefix##_tree_node_remove(AnvTreeNode* node, Size index) { \
        return anv_tree_node_remove(node, index);                       \
    }                                                                   \
                                                                        \
    FORCE_INLINE void anv_##prefix##_tree_node_delete(AnvTreeNode* node, Size index) { \
        anv_tree_node_delete(node, index);                              \
    }                                                                   \
                                                                        \
    FORCE_INLINE AnvTreeNode* anv_##prefix##_tree_node_push_front_fast(AnvTreeNode* node, type* p_data) { \
        return anv_tree_node_push_front_fast(node, (void*)p_data);      \
    }                                                                   \
                                                                        \
    FORCE_INLINE AnvTreeNode* anv_##prefix##_tree_node_insert_fast(AnvTreeNode* node, type* p_data, Size index) { \
        return anv_tree_node_insert_fast(node, (void*)p_data, index);   \
    }                                                                   \
                                                                        \
    FORCE_INLINE AnvTreeNode* anv_##prefix##_tree_node_remove_fast(AnvTreeNode* node, Size index) { \
        return anv_tree_node_remove_fast(node, index);                  \
    }                                                                   \
                                                                        \
    FORCE_INLINE void anv_##prefix##_tree_node_delete_fast(AnvTreeNode* node, Size index) { \
        anv_tree_node_delete_fast(node, index);                         \
    }


#endif //  ANV_UTILS_INTERFACE_TREE_H

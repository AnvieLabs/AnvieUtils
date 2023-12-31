/**
 * @file Common.h
 * @date Sat, 30th December, 2023
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
 * @brief Defines macros to declare type specific callbacks
 * for use in other interface builders.
 * */

#ifndef ANVIE_UTILS_CONTAINERS_INTEFACE_COMMON_H
#define ANVIE_UTILS_CONTAINERS_INTEFACE_COMMON_H

/**
 * Create type-specific callbacks for integer type containers.
 * */
#define DEF_INTEGER_TYPE_SPECIFIC_CALLBACKS(type, typename)             \
    typedef void (*Create##typename##CopyCallback)(type* dst, type src, void* udata); \
    typedef void (*Destroy##typename##CopyCallback)(type* copy, void* udata); \
    typedef void (*Print##typename##Callback)(type element, Size pos, void* udata); \
    typedef Bool (*Filter##typename##Callback)(type element, void* udata);  \
    typedef Int32 (*Compare##typename##Callback)(type p1, type p2, void* udata); \
    typedef Size (*Hash##typename##Callback)(type key, void* udata)

/**
 * Create type-specific callbacks for struct type containers.
 * */
#define DEF_STRUCT_TYPE_SPECIFIC_CALLBACKS(type, typename)             \
    typedef void (*Create##typename##CopyCallback)(type* dst, type* src, void* udata); \
    typedef void (*Destroy##typename##CopyCallback)(type* copy, void* udata); \
    typedef void (*Print##typename##Callback)(type* element, Size pos, void* udata); \
    typedef Bool (*Filter##typename##Callback)(type* element, void* udata);  \
    typedef Int32 (*Compare##typename##Callback)(type* p1, type* p2, void* udata); \
    typedef Size (*Hash##typename##Callback)(type* key, void* udata)

#endif // ANVIE_UTILS_CONTAINERS_INTEFACE_COMMON_H

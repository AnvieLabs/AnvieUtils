/**
 * @file VectorInterface.h
 * @date Mon, 14th July, 2023
 * @author Siddharth Mishra (admin@brightprogrammer.in)
 * @copyright Copyright (c) 2023 Anvie Labs
 *
 * Vector Interface Creator.
 * */


#ifndef ANV_UTILS_VECTOR_INTERFACE_H
#define ANV_UTILS_VECTOR_INTERFACE_H

#include <Anvie/HelperDefines.h>

#define DEF_ANV_INTEGER_VECTOR_INTERFACE(prefix, type)                \
    FORCE_INLINE AnvVector* Anv##prefix##Vector_Create() {          \
        return AnvVector_Create(sizeof(type), NULL, NULL);          \
    }                                                               \
                                                                    \
    FORCE_INLINE void Anv##prefix##Vector_Destroy(AnvVector* vec) {     \
        AnvVector_Destroy(vec);                                         \
    }                                                                   \
                                                                        \
    FORCE_INLINE void Anv##prefix##Vector_Resize(AnvVector* vec, Size sz) { \
        AnvVector_Resize(vec, sz);                                      \
    }                                                                   \
                                                                        \
    FORCE_INLINE void Anv##prefix##Vector_Clear(AnvVector* vec) {       \
        AnvVector_Clear(vec);                                           \
    }                                                                   \
                                                                        \
    FORCE_INLINE void Anv##prefix##Vector_Copy(AnvVector* vec, Size to, Size from) {        \
        AnvVector_Copy(vec, to, from);                                  \
    }                                                                   \
                                                                        \
    FORCE_INLINE void Anv##prefix##Vector_Move(AnvVector* vec, Size to, Size from) { \
        AnvVector_Move(vec, to, from);                                  \
    }                                                                   \
                                                                        \
    FORCE_INLINE void Anv##prefix##Vector_Overwrite(AnvVector* vec, Size to, type value) { \
        AnvVector_Overwrite(vec, to, (void*)(Uint64)value);             \
    }                                                                   \
                                                                        \
    FORCE_INLINE void Anv##prefix##Vector_Insert(AnvVector* vec, type value, Size pos) { \
        AnvVector_Insert(vec, (void*)(Uint64)value, pos);               \
    }                                                                   \
                                                                        \
    FORCE_INLINE void Anv##prefix##Vector_Delete(AnvVector* vec, Size pos) { \
        AnvVector_Delete(vec, pos);                                     \
    }                                                                   \
                                                                        \
    FORCE_INLINE type Anv##prefix##Vector_Remove(AnvVector* vec, Size pos) { \
        return (type)(Uint64)AnvVector_Remove(vec, pos);                \
    }                                                                   \
                                                                        \
    FORCE_INLINE void Anv##prefix##Vector_InsertFast(AnvVector* vec, type value, Size pos) { \
        AnvVector_InsertFast(vec, (void*)(Uint64)value, pos);           \
    }                                                                   \
                                                                        \
    FORCE_INLINE void Anv##prefix##Vector_DeleteFast(AnvVector* vec, Size pos) { \
        AnvVector_DeleteFast(vec, pos);                                 \
    }                                                                   \
                                                                        \
    FORCE_INLINE type Anv##prefix##Vector_RemoveFast(AnvVector* vec, Size pos) { \
        return (type)(Uint64)AnvVector_RemoveFast(vec, pos);            \
    }                                                                   \
                                                                        \
    FORCE_INLINE void Anv##prefix##Vector_PushFront(AnvVector* vec, type value) { \
        AnvVector_PushFront(vec, (void*)(Uint64)value);                 \
    }                                                                   \
                                                                        \
    FORCE_INLINE type Anv##prefix##Vector_PopFront(AnvVector* vec) {    \
        return (type)(Uint64)AnvVector_PopFront(vec);                   \
    }                                                                   \
                                                                        \
    FORCE_INLINE void Anv##prefix##Vector_PushFrontFast(AnvVector* vec, type value) { \
        AnvVector_PushFrontFast(vec, (void*)(Uint64)value);             \
    }                                                                   \
                                                                        \
    FORCE_INLINE type Anv##prefix##Vector_PopFrontFast(AnvVector* vec) { \
        return (type)(Uint64)AnvVector_PopFrontFast(vec);               \
    }                                                                   \
                                                                        \
    FORCE_INLINE void Anv##prefix##Vector_PushBack(AnvVector* vec, type value) { \
        AnvVector_PushBack(vec, (void*)(Uint64)value);                  \
    }                                                                   \
                                                                        \
    FORCE_INLINE type Anv##prefix##Vector_PopBack(AnvVector* vec) {     \
        return (type)(Uint64)AnvVector_PopBack(vec);                    \
    }                                                                   \
                                                                        \
    FORCE_INLINE type Anv##prefix##Vector_Peek(AnvVector* vec, Size pos) {\
        return (type)(Uint64)AnvVector_Peek(vec, pos);                  \
    }                                                                   \
                                                                        \
    FORCE_INLINE type Anv##prefix##Vector_Front(AnvVector* vec) {       \
        return (type)(Uint64)AnvVector_Front(vec);                      \
    }                                                                   \
                                                                        \
    FORCE_INLINE type Anv##prefix##Vector_Back(AnvVector* vec) {        \
        return (type)(Uint64)AnvVector_Back(vec);                       \
    }                                                                   \
                                                                        \
    FORCE_INLINE void Anv##prefix##Vector_Print(AnvVector* vec, AnvPrintElementCallback printer) { \
        AnvVector_Print(vec, printer);                                  \
    }                                                                   \
                                                                        \
    typedef AnvVector Anv##prefix##Vector


#define DEF_ANV_STRUCT_VECTOR_INTERFACE(prefix, type, copy_create, copy_destroy) \
    FORCE_INLINE AnvVector* Anv##prefix##Vector_Create() {              \
        return AnvVector_Create(sizeof(type), (copy_create), (copy_destroy)); \
    }                                                                   \
                                                                        \
    FORCE_INLINE void Anv##prefix##Vector_Destroy(AnvVector* vec) {     \
        AnvVector_Destroy(vec);                                         \
    }                                                                   \
                                                                        \
    FORCE_INLINE void Anv##prefix##Vector_Resize(AnvVector* vec, Size sz) { \
        AnvVector_Resize(vec, sz);                                      \
    }                                                                   \
                                                                        \
    FORCE_INLINE void Anv##prefix##Vector_Clear(AnvVector* vec) {       \
        AnvVector_Clear(vec);                                           \
    }                                                                   \
                                                                        \
    FORCE_INLINE void Anv##prefix##Vector_Copy(AnvVector* vec, Size to, Size from) { \
        AnvVector_Copy(vec, to, from);                                  \
    }                                                                   \
                                                                        \
    FORCE_INLINE void Anv##prefix##Vector_Move(AnvVector* vec, Size to, Size from) { \
        AnvVector_Move(vec, to, from);                                  \
    }                                                                   \
                                                                        \
    FORCE_INLINE void Anv##prefix##Vector_Overwrite(AnvVector* vec, Size to, type* p_data) { \
        AnvVector_Overwrite(vec, to, (void*)p_data);                    \
    }                                                                   \
                                                                        \
    FORCE_INLINE void Anv##prefix##Vector_Insert(AnvVector* vec, type* p_data, Size pos) { \
        AnvVector_Insert(vec, (void*)p_data, pos);                      \
    }                                                                   \
                                                                        \
    FORCE_INLINE void Anv##prefix##Vector_Delete(AnvVector* vec, Size pos) { \
        AnvVector_Delete(vec, pos);                                     \
    }                                                                   \
                                                                        \
    FORCE_INLINE type* Anv##prefix##Vector_Remove(AnvVector* vec, Size pos) { \
        return (type*)(Uint64)AnvVector_Remove(vec, pos);                \
    }                                                                   \
                                                                        \
    FORCE_INLINE void Anv##prefix##Vector_InsertFast(AnvVector* vec, type* p_data, Size pos) { \
        AnvVector_InsertFast(vec, (void*)p_data, pos);                  \
    }                                                                   \
                                                                        \
    FORCE_INLINE void Anv##prefix##Vector_DeleteFast(AnvVector* vec, Size pos) { \
        AnvVector_DeleteFast(vec, pos);                                 \
    }                                                                   \
                                                                        \
    FORCE_INLINE type* Anv##prefix##Vector_RemoveFast(AnvVector* vec, Size pos) { \
        return (type*)(Uint64)AnvVector_RemoveFast(vec, pos);            \
    }                                                                   \
                                                                        \
    FORCE_INLINE void Anv##prefix##Vector_PushFront(AnvVector* vec, type* p_data) { \
        AnvVector_PushFront(vec, (void*)p_data);                        \
    }                                                                   \
                                                                        \
    FORCE_INLINE type* Anv##prefix##Vector_PopFront(AnvVector* vec) {    \
        return (type*)(Uint64)AnvVector_PopFront(vec);                   \
    }                                                                   \
                                                                        \
    FORCE_INLINE void Anv##prefix##Vector_PushFrontFast(AnvVector* vec, type* p_data) { \
        AnvVector_PushFrontFast(vec, (void*)p_data);                    \
    }                                                                   \
                                                                        \
    FORCE_INLINE type* Anv##prefix##Vector_PopFrontFast(AnvVector* vec) { \
        return (type*)(Uint64)AnvVector_PopFrontFast(vec);               \
    }                                                                   \
                                                                        \
    FORCE_INLINE void Anv##prefix##Vector_PushBack(AnvVector* vec, type* p_data) { \
        AnvVector_PushBack(vec, (void*)p_data);                         \
    }                                                                   \
                                                                        \
    FORCE_INLINE type* Anv##prefix##Vector_PopBack(AnvVector* vec) {     \
        return (type*)(Uint64)AnvVector_PopBack(vec);                    \
    }                                                                   \
                                                                        \
    FORCE_INLINE type* Anv##prefix##Vector_Peek(AnvVector* vec, Size pos) {\
        return (type*)(Uint64)AnvVector_Peek(vec, pos);                  \
    }                                                                   \
                                                                        \
    FORCE_INLINE type* Anv##prefix##Vector_Front(AnvVector* vec) {       \
        return (type*)(Uint64)AnvVector_Front(vec);                      \
    }                                                                   \
                                                                        \
    FORCE_INLINE type* Anv##prefix##Vector_Back(AnvVector* vec) {        \
        return (type*)(Uint64)AnvVector_Back(vec);                       \
    }                                                                   \
                                                                        \
    FORCE_INLINE void Anv##prefix##Vector_Print(AnvVector* vec, AnvPrintElementCallback printer) { \
        AnvVector_Print(vec, printer);                                  \
    }                                                                   \
                                                                        \
    typedef AnvVector Anv##prefix##Vector

#endif // ANV_UTILS_VECTOR_INTERFACE_H

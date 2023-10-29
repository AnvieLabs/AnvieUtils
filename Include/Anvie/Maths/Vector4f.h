/**
 * @file Vector4f.h
 * @author Siddharth Mishra (admin@brightprogrammer.in)
 * @date 6th Aug 2023
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
 * @brief Float32 Vector of 4 components.
 * */

#ifndef ANVIE_UTILS_MATH_VECTOR4F_H
#define ANVIE_UTILS_MATH_VECTOR4F_H

#include <Anvie/Types.h>

typedef struct anvie_vector_4f_t {
    Float32 x;
    Float32 y;
    Float32 z;
    Float32 t;
} Vector4f;

Vector4f* vector_4f_create(Float32 x, Float32 y, Float32 z, Float32 t);
void vector_4f_destroy(Vector4f* p_vec);
Vector4f* vector_4f_create_copy(Vector4f* p_vec);

Vector4f* vector_4f_origin();
Vector4f* vector_4f_x_axis();
Vector4f* vector_4f_y_axis();
Vector4f* vector_4f_z_axis();
Vector4f* vector_4f_t_axis();

Vector4f* vector_4f_add(Vector4f* p_vec1, Vector4f* p_vec2);
Vector4f* vector_4f_sub(Vector4f* p_vec1, Vector4f* p_vec2);
Vector4f* vector_4f_scale(Vector4f* p_vec, Float32 scale);
Float32 vector_4f_dot(Vector4f* p_vec1, Vector4f* p_vec2);

Float32 vector_4f_compute_norm(Vector4f* p_vec);
void vector_4f_normalize(Vector4f* p_vec);

#endif // ANVIE_UTILS_MATH_VECTOR4F_H

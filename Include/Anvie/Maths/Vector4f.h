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
} AnvVector4f;

AnvVector4f* anv_vector_4f_create(Float32 x, Float32 y, Float32 z, Float32 t);
void anv_vector_4f_destroy(AnvVector4f* p_vec);
AnvVector4f* anv_vector_4f_create_copy(AnvVector4f* p_vec);

AnvVector4f* anv_vector_4f_origin();
AnvVector4f* anv_vector_4f_x_axis();
AnvVector4f* anv_vector_4f_y_axis();
AnvVector4f* anv_vector_4f_z_axis();
AnvVector4f* anv_vector_4f_t_axis();

AnvVector4f* anv_vector_4f_add(AnvVector4f* p_vec1, AnvVector4f* p_vec2);
AnvVector4f* anv_vector_4f_sub(AnvVector4f* p_vec1, AnvVector4f* p_vec2);
AnvVector4f* anv_vector_4f_scale(AnvVector4f* p_vec, Float32 scale);
Float32 anv_vector_4f_dot(AnvVector4f* p_vec1, AnvVector4f* p_vec2);

Float32 anv_vector_4f_compute_norm(AnvVector4f* p_vec);
void anv_vector_4f_normalize(AnvVector4f* p_vec);

#endif // ANVIE_UTILS_MATH_VECTOR4F_H

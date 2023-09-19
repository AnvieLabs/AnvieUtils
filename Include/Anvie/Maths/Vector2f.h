/**
 * @file Vector2f.h
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

#ifndef ANVIE_UTILS_MATH_VECTOR2F_H
#define ANVIE_UTILS_MATH_VECTOR2F_H

#include <Anvie/Types.h>

typedef struct anvie_vector_2f_t {
    Float32 x;
    Float32 y;
} AnvVector2f;

AnvVector2f* anv_vector_2f_create(Float32 x, Float32 y);
void anv_vector_2f_destroy(AnvVector2f* p_vec);
AnvVector2f* anv_vector_2f_create_copy(AnvVector2f* p_vec);

AnvVector2f* anv_vector_2f_origin();
AnvVector2f* anv_vector_2f_x_axis();
AnvVector2f* anv_vector_2f_y_axis();

AnvVector2f* anv_vector_2f_add(AnvVector2f* p_vec1, AnvVector2f* p_vec2);
AnvVector2f* anv_vector_2f_sub(AnvVector2f* p_vec1, AnvVector2f* p_vec2);
AnvVector2f* anv_vector_2f_scale(AnvVector2f* p_vec, Float32 scale);
Float32 anv_vector_2f_dot(AnvVector2f* p_vec1, AnvVector2f* p_vec2);

Float32 anv_vector_2f_compute_norm(AnvVector2f* p_vec);
void anv_vector_2f_normalize(AnvVector2f* p_vec);

#endif // ANVIE_UTILS_MATH_VECTOR2F_H

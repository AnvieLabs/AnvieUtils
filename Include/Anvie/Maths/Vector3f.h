/**
 * @file Vector3f.h
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

#ifndef ANVIE_UTILS_MATH_VECTOR3F_H
#define ANVIE_UTILS_MATH_VECTOR3F_H

#include <Anvie/Types.h>

typedef struct anvie_vector_3f_t {
    Float32 x;
    Float32 y;
    Float32 z;
} AnvVector3f;

AnvVector3f* anv_vector_3f_create(Float32 x, Float32 y, Float32 z);
void anv_vector_3f_destroy(AnvVector3f* p_vec);
AnvVector3f* anv_vector_3f_create_copy(AnvVector3f* p_vec);

AnvVector3f* anv_vector_3f_origin();
AnvVector3f* anv_vector_3f_x_axis();
AnvVector3f* anv_vector_3f_y_axis();
AnvVector3f* anv_vector_3f_z_axis();

AnvVector3f* anv_vector_3f_add(AnvVector3f* p_vec1, AnvVector3f* p_vec2);
AnvVector3f* anv_vector_3f_sub(AnvVector3f* p_vec1, AnvVector3f* p_vec2);
AnvVector3f* anv_vector_3f_scale(AnvVector3f* p_vec, Float32 scale);
Float32 anv_vector_3f_dot(AnvVector3f* p_vec1, AnvVector3f* p_vec2);
AnvVector3f* anv_vector_3f_cross(AnvVector3f* p_vec1, AnvVector3f* p_vec2);

Float32 anv_vector_3f_compute_norm(AnvVector3f* p_vec);
void anv_vector_3f_normalize(AnvVector3f* p_vec);

#endif // ANVIE_UTILS_MATH_VECTOR3F_H

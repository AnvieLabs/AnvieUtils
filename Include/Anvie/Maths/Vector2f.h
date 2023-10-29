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
} Vector2f;

Vector2f* vector_2f_create(Float32 x, Float32 y);
void vector_2f_destroy(Vector2f* vec);
Vector2f* vector_2f_create_copy(Vector2f* vec);

Vector2f* vector_2f_origin();
Vector2f* vector_2f_x_axis();
Vector2f* vector_2f_y_axis();

Vector2f* vector_2f_add(Vector2f* vec1, Vector2f* vec2);
Vector2f* vector_2f_sub(Vector2f* vec1, Vector2f* vec2);
Vector2f* vector_2f_scale(Vector2f* vec, Float32 scale);
Float32 vector_2f_dot(Vector2f* vec1, Vector2f* vec2);

Float32 vector_2f_compute_norm(Vector2f* vec);
void vector_2f_normalize(Vector2f* vec);

#endif // ANVIE_UTILS_MATH_VECTOR2F_H

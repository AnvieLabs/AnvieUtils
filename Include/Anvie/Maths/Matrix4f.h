/**
 * @file Matrix4f.h
 * @author Siddharth Mishra (admin@brightprogrammer.in)
 * @date 3rd Sept 2023
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
 * @brief 4x4 Float32 matrix.
 * */

#ifndef ANVIE_UTILS_MATHS_MATRIX4F_H
#define ANVIE_UTILS_MATHS_MATRIX4F_H

#include <Anvie/Types.h>

/**
 * @brief 4x4 Matrix of Float32 values.
 * Used in computing Model, View, Project matrix.
 * */
typedef struct anvie_matrix_4f_t {
    Float32 data[4][4];
} AnvMatrix4f;

AnvMatrix4f* anv_matrix_4f_create();
void anv_matrix_4f_destroy(AnvMatrix4f* p_mat);

void anv_matrix_4f_mul(AnvMatrix4f* p_mat1, AnvMatrix4f* p_mat2);
void anv_matrix_4f_add(AnvMatrix4f* p_mat1, AnvMatrix4f* p_mat2);
void anv_matrix_4f_sub(AnvMatrix4f* p_mat1, AnvMatrix4f* p_mat2);

AnvMatrix4f* anv_matrix_4f_identity();
AnvMatrix4f* anv_matrix_4f_projection_perspective(
    Float32 aspect, Float32 fov,
    Float32 far, Float32 near
);
AnvMatrix4f* anv_matrix_4f_projection_orthographic(
    Float32 right, Float32 left,
    Float32 top, Float32 bottom,
    Float32 far, Float32 near
);
AnvMatrix4f* anv_matrix_4f_look_at(
    Float32 eyeX, Float32 eyeY, Float32 eyeZ,
    Float32 targetX, Float32 targetY, Float32 targetZ,
    Float32 upX, Float32 upY, Float32 upZ
);
AnvMatrix4f* anv_matrix_4f_translation_matrix(Float32 dx, Float32 dy, Float32 dz);
AnvMatrix4f* anv_matrix_4f_rotation_matrix(Float32 yaw, Float32 pitch, Float32 roll);
AnvMatrix4f* anv_matrix_4f_scale_matrix(Float32 sx, Float32 sy, Float32 sz);

void anv_matrix_4f_translate(AnvMatrix4f* p_matrix, Float32 dx, Float32 dy, Float32 dz);
void anv_matrix_4f_rotate(AnvMatrix4f* p_matrix, Float32 yaw, Float32 pitch, Float32 roll);
void anv_matrix_4f_scale(AnvMatrix4f* p_matrix, Float32 sx, Float32 sy, Float32 sz);

#endif // ANVIE_UTILS_MATHS_MATRIX4F_H

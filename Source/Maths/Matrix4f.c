/**
 * @file Matrix4f.c
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

#include <Anvie/Maths/Matrix4f.h>
#include <Anvie/Maths/Vector3f.h>
#include <math.h>

#include <Anvie/HelperDefines.h>

/**
 * Create a new Matrix4f.
 *
 * @return AnvMatrix4f on success, NULL otherwise.
 * */
AnvMatrix4f* anv_matrix_4f_create() {
    AnvMatrix4f* p_mat = NEW(AnvMatrix4f);
    RETURN_VALUE_IF_FAIL(p_mat, NULL, ERR_OUT_OF_MEMORY);

    return p_mat;
}

/**
 * Destroy given Matrxi4f
 *
 * @param p_mat
 * */
void anv_matrix_4f_destroy(AnvMatrix4f* p_mat) {
    FREE(p_mat);
}

/**
 * Multiply first matrix with other and store the result in first matrix.
 *
 * @param p_mat1
 * @param p_mat2
 * */
void anv_matrix_4f_mul(AnvMatrix4f* p_mat1, AnvMatrix4f* p_mat2) {
    RETURN_IF_FAIL(p_mat1 && p_mat2, ERR_INVALID_ARGUMENTS);

#define M1(r, c) p_mat1->data[r][c]
#define M2(r, c) p_mat2->data[r][c]

    M1(0, 0) = M1(0, 0)*M2(0, 0) + M1(0, 1)*M2(1, 0) + M1(0, 2)*M2(2, 0) + M1(0, 3)*M2(3, 0);
    M1(0, 1) = M1(0, 0)*M2(0, 1) + M1(0, 1)*M2(1, 1) + M1(0, 2)*M2(2, 1) + M1(0, 3)*M2(3, 1);
    M1(0, 2) = M1(0, 0)*M2(0, 2) + M1(0, 1)*M2(1, 2) + M1(0, 2)*M2(2, 2) + M1(0, 3)*M2(3, 2);
    M1(0, 3) = M1(0, 0)*M2(0, 3) + M1(0, 1)*M2(1, 3) + M1(0, 2)*M2(2, 3) + M1(0, 3)*M2(3, 3);

    M1(1, 0) = M1(1, 0)*M2(0, 0) + M1(1, 1)*M2(1, 0) + M1(1, 2)*M2(2, 0) + M1(1, 3)*M2(3, 0);
    M1(1, 1) = M1(1, 0)*M2(0, 1) + M1(1, 1)*M2(1, 1) + M1(1, 2)*M2(2, 1) + M1(1, 3)*M2(3, 1);
    M1(1, 2) = M1(1, 0)*M2(0, 2) + M1(1, 1)*M2(1, 2) + M1(1, 2)*M2(2, 2) + M1(1, 3)*M2(3, 2);
    M1(1, 3) = M1(1, 0)*M2(0, 3) + M1(1, 1)*M2(1, 3) + M1(1, 2)*M2(2, 3) + M1(1, 3)*M2(3, 3);

    M1(2, 0) = M1(2, 0)*M2(0, 0) + M1(2, 1)*M2(1, 0) + M1(2, 2)*M2(2, 0) + M1(2, 3)*M2(3, 0);
    M1(2, 1) = M1(2, 0)*M2(0, 1) + M1(2, 1)*M2(1, 1) + M1(2, 2)*M2(2, 1) + M1(2, 3)*M2(3, 1);
    M1(2, 2) = M1(2, 0)*M2(0, 2) + M1(2, 1)*M2(1, 2) + M1(2, 2)*M2(2, 2) + M1(2, 3)*M2(3, 2);
    M1(2, 3) = M1(2, 0)*M2(0, 3) + M1(2, 1)*M2(1, 3) + M1(2, 2)*M2(2, 3) + M1(2, 3)*M2(3, 3);

    M1(3, 0) = M1(3, 0)*M2(0, 0) + M1(3, 1)*M2(1, 0) + M1(3, 2)*M2(2, 0) + M1(3, 3)*M2(3, 0);
    M1(3, 1) = M1(3, 0)*M2(0, 1) + M1(3, 1)*M2(1, 1) + M1(3, 2)*M2(2, 1) + M1(3, 3)*M2(3, 1);
    M1(3, 2) = M1(3, 0)*M2(0, 2) + M1(3, 1)*M2(1, 2) + M1(3, 2)*M2(2, 2) + M1(3, 3)*M2(3, 2);
    M1(3, 3) = M1(3, 0)*M2(0, 3) + M1(3, 1)*M2(1, 3) + M1(3, 2)*M2(2, 3) + M1(3, 3)*M2(3, 3);

#undef M1
#undef M2
}

/**
 * Add second matrix to first and store the result in first matrix.
 *
 * @param p_mat1
 * @param p_mat2
 * */
void anv_matrix_4f_add(AnvMatrix4f* p_mat1, AnvMatrix4f* p_mat2) {
    RETURN_IF_FAIL(p_mat1 && p_mat2, ERR_INVALID_ARGUMENTS);

#define M1(r, c) p_mat1->data[r][c]
#define M2(r, c) p_mat2->data[r][c]

    M1(0, 0) = M1(0, 0) + M2(0, 0);
    M1(0, 1) = M1(0, 1) + M2(0, 1);
    M1(0, 2) = M1(0, 2) + M2(0, 2);
    M1(0, 3) = M1(0, 3) + M2(0, 3);

    M1(1, 0) = M1(1, 0) + M2(1, 0);
    M1(1, 1) = M1(1, 1) + M2(1, 1);
    M1(1, 2) = M1(1, 2) + M2(1, 2);
    M1(1, 3) = M1(1, 3) + M2(1, 3);

    M1(2, 0) = M1(2, 0) + M2(2, 0);
    M1(2, 1) = M1(2, 1) + M2(2, 1);
    M1(2, 2) = M1(2, 2) + M2(2, 2);
    M1(2, 3) = M1(2, 3) + M2(2, 3);

    M1(3, 0) = M1(3, 0) + M2(3, 0);
    M1(3, 1) = M1(3, 1) + M2(3, 1);
    M1(3, 2) = M1(3, 2) + M2(3, 2);
    M1(3, 3) = M1(3, 3) + M2(3, 3);

#undef M1
#undef M2
}

/**
 * Subtract second matrix from first and store the result in first matrix.
 *
 * @param p_mat1
 * @param p_mat2
 * */
void anv_matrix_4f_sub(AnvMatrix4f* p_mat1, AnvMatrix4f* p_mat2) {
    RETURN_IF_FAIL(p_mat1 && p_mat2, ERR_INVALID_ARGUMENTS);

#define M1(r, c) p_mat1->data[r][c]
#define M2(r, c) p_mat2->data[r][c]

    M1(0, 0) = M1(0, 0) - M2(0, 0);
    M1(0, 1) = M1(0, 1) - M2(0, 1);
    M1(0, 2) = M1(0, 2) - M2(0, 2);
    M1(0, 3) = M1(0, 3) - M2(0, 3);

    M1(1, 0) = M1(1, 0) - M2(1, 0);
    M1(1, 1) = M1(1, 1) - M2(1, 1);
    M1(1, 2) = M1(1, 2) - M2(1, 2);
    M1(1, 3) = M1(1, 3) - M2(1, 3);

    M1(2, 0) = M1(2, 0) - M2(2, 0);
    M1(2, 1) = M1(2, 1) - M2(2, 1);
    M1(2, 2) = M1(2, 2) - M2(2, 2);
    M1(2, 3) = M1(2, 3) - M2(2, 3);

    M1(3, 1) = M1(3, 1) - M2(3, 1);
    M1(3, 0) = M1(3, 0) - M2(3, 0);
    M1(3, 2) = M1(3, 2) - M2(3, 2);
    M1(3, 3) = M1(3, 3) - M2(3, 3);

#undef M1
#undef M2
}

/**
 * Create identity matrix.
 *
 * @return AnvMatrix4f on success, NULL otherwise.
 * */
AnvMatrix4f* anv_matrix_4f_identity() {
    AnvMatrix4f* p_mat = anv_matrix_4f_create();
    RETURN_VALUE_IF_FAIL(p_mat, NULL, "Failed to create identity matrix\n");

    p_mat->data[0][0] = 1.f;
    p_mat->data[1][1] = 1.f;
    p_mat->data[2][2] = 1.f;
    p_mat->data[3][3] = 1.f;

    return p_mat;
}

/**
 * Create a new perspective projection matrix
 *
 * @param aspect Aspect ratio
 * @param fov Angle of field of view in radians
 * @param far Far plane
 * @param near Near plane
 *
 * @return AnvMatrix4f* on success, NULL otherwise.
 * */
AnvMatrix4f* anv_matrix_4f_projection_perspective(
    Float32 aspect, Float32 fov,
    Float32 far, Float32 near) {
    AnvMatrix4f* p_mat = anv_matrix_4f_create();
    RETURN_VALUE_IF_FAIL(p_mat, NULL, "Failed to create perspective projection matrix\n");

#define M(r, c) p_mat->data[r][c]

    M(0, 0) = 1/(aspect*tan(fov/2));

    M(1, 1) = 1/tan(fov/2);

    M(2, 2) = -(far + near)/(far-near);
    M(2, 3) = -2*far*near/(far-near);

    M(3, 2) = -1;

#undef M

    return p_mat;
}

/**
 * Create a new orthographic projection matrix
 *
 * @param right
 * @param left
 * @param top
 * @param bottom
 * @param far
 * @param near
 *
 * @return AnvMatrix4f* on success, NULL otherwise.
 * */
AnvMatrix4f* anv_matrix_4f_projection_orthographic(
    Float32 right, Float32 left,
    Float32 top, Float32 bottom,
    Float32 far, Float32 near) {
    AnvMatrix4f* p_mat = anv_matrix_4f_create();
    RETURN_VALUE_IF_FAIL(p_mat, NULL, "Failed to create perspective projection matrix\n");

#define M(r, c) p_mat->data[r][c]

    M(0, 0) = 2/(right - left);
    M(0, 3) = -(right + left)/(right - left);

    M(1, 1) = 2/(top - bottom);
    M(1, 3) = -(top + bottom)/(top - bottom);

    M(2, 2) = -2/(far - near);
    M(2, 3) = -(far + near)/(far - near);

    M(3, 3) = 1;

#undef M

    return p_mat;
}

/**
 * Create a LookAt matrix
 *
 * @param eyeX
 * @param eyeY
 * @param eyeZ
 * @param targetX
 * @param targetY
 * @param targetZ
 * @param upX
 * @param upY
 * @param upZ
 *
 * @return AnvMatrix4f* on success, NULL otherwise.
 * */
AnvMatrix4f* anv_matrix_4f_look_at(
    Float32 eyeX, Float32 eyeY, Float32 eyeZ,
    Float32 targetX, Float32 targetY, Float32 targetZ,
    Float32 upX, Float32 upY, Float32 upZ
) {
    AnvMatrix4f* p_mat = anv_matrix_4f_identity();
    RETURN_VALUE_IF_FAIL(p_mat, NULL, "Failed to create LookAt matrix\n");

    AnvVector3f* eye = anv_vector_3f_create(eyeX, eyeY, eyeZ);
    AnvVector3f* target = anv_vector_3f_create(targetX, targetY, targetZ);
    AnvVector3f* up = anv_vector_3f_create(upX, upY, upZ);

    AnvVector3f* fwd = anv_vector_3f_sub(eye, target);
    AnvVector3f* right = anv_vector_3f_cross(up, fwd);

    anv_vector_3f_destroy(up);
    up = anv_vector_3f_cross(fwd, right);

    anv_vector_3f_normalize(fwd);
    anv_vector_3f_normalize(right);

#define M(r, c) p_mat->data[r][c]
    M(0, 0) = right->x;
    M(1, 0) = right->y;
    M(2, 0) = right->z;

    M(0, 1) = up->x;
    M(1, 1) = up->y;
    M(2, 1) = up->z;

    M(0, 2) = -fwd->x;
    M(1, 2) = -fwd->y;
    M(2, 2) = -fwd->z;
#undef M

    anv_vector_3f_destroy(eye);
    anv_vector_3f_destroy(target);
    anv_vector_3f_destroy(up);
    anv_vector_3f_destroy(fwd);
    anv_vector_3f_destroy(right);

    anv_matrix_4f_translate(p_mat, eyeX, eyeY, eyeZ);

    return p_mat;
}

/**
 * Create translation matrix.
 *
 * @param dx Change in X coordinate
 * @param dy Change in Y coordinate
 * @param dz Change in Z coordinate
 *
 * @return AnvMatrix4f* on success, NULL otherwise.
 * */
AnvMatrix4f* anv_matrix_4f_translation_matrix(Float32 dx, Float32 dy, Float32 dz) {
    AnvMatrix4f* p_mat = anv_matrix_4f_identity();
    RETURN_VALUE_IF_FAIL(p_mat, NULL, "Failed to create translation matrix\n");

#define M(r, c) p_mat->data[r][c]
    M(0, 0) = dx;
    M(1, 0) = dy;
    M(2, 0) = dz;
    M(4, 0) = 1;
#undef M

    return p_mat;
}

/**
 * Create rotation matrix for given rotation angles.
 * All angles must be in radians.
 *
 * @param yaw Rotation angle about Z axis
 * @param pitch Rotation angle about Y axis
 * @param roll Rotation angle about X axis
 *
 * @return AnvMatrix4f* on success, NULL otherwise.
 * */
AnvMatrix4f* anv_matrix_4f_rotation_matrix(Float32 yaw, Float32 pitch, Float32 roll) {
    AnvMatrix4f* p_mat = anv_matrix_4f_identity();
    RETURN_VALUE_IF_FAIL(p_mat, NULL, "Failed to create rotation matrix\n");

#define M(r, c) p_mat->data[r][c]
    M(0, 0) = cos(pitch)*cos(yaw);
    M(0, 1) = sin(roll)*sin(pitch)*cos(yaw) - cos(roll)*cos(yaw);
    M(0, 2) = cos(roll)*sin(pitch)*cos(yaw) + sin(roll)*sin(yaw);

    M(1, 0) = cos(pitch)*sin(yaw);
    M(1, 1) = sin(roll)*sin(pitch)*sin(yaw) + cos(roll)*cos(yaw);
    M(1, 2) = cos(roll)*sin(pitch)*sin(yaw) - sin(roll)*sin(yaw);

    M(2, 0) = -sin(pitch);
    M(2, 1) = sin(roll)*cos(pitch);
    M(2, 2) = cos(roll)*cos(pitch);
#undef M

    return p_mat;
}

/**
 * Create scale matrix for given scale values.
 *
 * @param sx Scale X axis.
 * @param sy Scale Y axis.
 * @param sz Scale Z axis.
 *
 * @return AnvMatrix4f* on success, NULL otherwise.
 * */
AnvMatrix4f* anv_matrix_4f_scale_matrix(Float32 sx, Float32 sy, Float32 sz) {
    AnvMatrix4f* p_mat = anv_matrix_4f_identity();
    RETURN_VALUE_IF_FAIL(p_mat, NULL, "Failed to create scale matrix\n");

#define M(r, c) p_mat->data[r][c]
    M(0, 0) = sx;
    M(1, 1) = sy;
    M(2, 2) = sz;
#undef M

    return p_mat;
}

/**
 * Translate given matrix by given amounts
 *
 * @param p_mat
 * @param dx Change in X axis
 * @param dy Change in Y axis
 * @param dz Change in Z axis
 * */
void anv_matrix_4f_translate(AnvMatrix4f* p_mat, Float32 dx, Float32 dy, Float32 dz) {
    RETURN_IF_FAIL(p_mat, "Failed to create translation matrix\n");
    AnvMatrix4f* p_trmat = anv_matrix_4f_translation_matrix(dx, dy, dz);
    RETURN_IF_FAIL(p_trmat, "Failed to create translation matrix\n");
    anv_matrix_4f_mul(p_mat, p_trmat);
    anv_matrix_4f_destroy(p_trmat);
}

/**
 * Rotate given matrix by given rotation angles.
 * All angles must be in radians.
 *
 * @param p_mat
 * @param yaw Rotation about Z axis.
 * @param pitch Rotation about Y axis.
 * @param roll Rotation about X axis.
 * */
void anv_matrix_4f_rotate(AnvMatrix4f* p_mat, Float32 yaw, Float32 pitch, Float32 roll) {
    RETURN_IF_FAIL(p_mat, "Failed to create translation matrix\n");
    AnvMatrix4f* p_rotmat = anv_matrix_4f_rotation_matrix(yaw, pitch, roll);
    RETURN_IF_FAIL(p_rotmat, "Failed to create rotation matrix\n");
    anv_matrix_4f_mul(p_mat, p_rotmat);
    anv_matrix_4f_destroy(p_rotmat);
}

/**
 * Rotate given matrix by given rotation angles.
 * All angles must be in radians.
 *
 * @param p_mat
 * @param sx Scale about X axis.
 * @param sy Scale about Y axis.
 * @param sz Scale about Z axis.
 * */
void anv_matrix_4f_scale(AnvMatrix4f* p_mat, Float32 sx, Float32 sy, Float32 sz) {
    RETURN_IF_FAIL(p_mat, "Failed to create translation matrix\n");
    AnvMatrix4f* p_scalemat = anv_matrix_4f_scale_matrix(sx, sy, sz);
    RETURN_IF_FAIL(p_scalemat, "Failed to create scale matrix\n");
    anv_matrix_4f_mul(p_mat, p_scalemat);
    anv_matrix_4f_destroy(p_scalemat);
}

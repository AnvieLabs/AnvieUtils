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
 * @return Matrix4f on success, NULL otherwise.
 * */
Matrix4f* matrix_4f_create() {
    Matrix4f* p_mat = NEW(Matrix4f);
    RETURN_VALUE_IF_FAIL(p_mat, NULL, ERR_OUT_OF_MEMORY);

    return p_mat;
}

/**
 * Destroy given Matrxi4f
 *
 * @param p_mat
 * */
void matrix_4f_destroy(Matrix4f* p_mat) {
    FREE(p_mat);
}

/**
 * Multiply first matrix with other and store the result in first matrix.
 *
 * @param p_mat1
 * @param p_mat2
 * */
void matrix_4f_mul(Matrix4f* p_mat1, Matrix4f* p_mat2) {
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
void matrix_4f_add(Matrix4f* p_mat1, Matrix4f* p_mat2) {
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
void matrix_4f_sub(Matrix4f* p_mat1, Matrix4f* p_mat2) {
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
 * @return Matrix4f on success, NULL otherwise.
 * */
Matrix4f* matrix_4f_identity() {
    Matrix4f* p_mat = matrix_4f_create();
    RETURN_VALUE_IF_FAIL(p_mat, NULL, "Failed to create identity matrix\n");

    p_mat->data[0][0] = 1.f;
    p_mat->data[1][1] = 1.f;
    p_mat->data[2][2] = 1.f;
    p_mat->data[3][3] = 1.f;

    return p_mat;
}

/**
 * Create a new frustum matrix
 *
 * @param right Value between -1 and 0
 * @param left Value between 0 and 1
 * @param top Value between -1 and 0
 * @param bottom Value between 0 and 1
 * @param far Value between 0 and 1
 * @param near Value between 0 and 1
 *
 * @return Matrix4f* on success, NULL otherwise.
 * */
Matrix4f* matrix_4f_frustum(
    Float32 left, Float32 right,
    Float32 top, Float32 bottom,
    Float32 near, Float32 far)
{
    RETURN_VALUE_IF_FAIL((left < right) && (top < bottom) && (near < far), NULL, ERR_INVALID_ARGUMENTS);

    Matrix4f* p_mat = matrix_4f_create();
    RETURN_VALUE_IF_FAIL(p_mat, NULL, "Failed to create perspective projection matrix\n");

#define M(r, c) p_mat->data[r][c]

    M(0, 0) = 2*near/(right-left);
    M(0, 2) = (right+left)/(right-left);

    M(1, 1) = 2*near/(top-bottom);
    M(1, 2) = (top+bottom)/(top-bottom);

    M(2, 2) = (far+near)/(near-far);
    M(2, 3) = 2*far*near/(near-far);

    M(3, 2) = -1;

#undef M

    return p_mat;
}

/**
 * Create a new perspective projection matrix
 *
 * @param aspect Widgh:Height aspect reation
 * @param fov Field of view in degrees
 * @param near Near plane
 * @param far Far plane
 * */
Matrix4f* matrix_4f_projection_perspective(
    Float32 aspect, Float32 fov,
    Float32 near, Float32 far
) {
    RETURN_VALUE_IF_FAIL((aspect != 0) && (fov != 0) && (near < far), NULL, ERR_INVALID_ARGUMENTS);

    Float32 scale = tan(fov * 0.5 * M_PI / 180) * near;
    Float32 r = fov * scale;
    Float32 l = -r;
    Float32 t = scale;
    Float32 b = -t;

    return matrix_4f_frustum(l, r, t, b, near, far);
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
 * @return Matrix4f* on success, NULL otherwise.
 * */
Matrix4f* matrix_4f_projection_orthographic(
    Float32 right, Float32 left,
    Float32 top, Float32 bottom,
    Float32 far, Float32 near) {
    Matrix4f* p_mat = matrix_4f_create();
    RETURN_VALUE_IF_FAIL(p_mat, NULL, "Failed to create orthographic projection matrix\n");

#define M(r, c) p_mat->data[r][c]

    M(0, 0) = 2*near/(right - left);
    M(0, 3) = -(right + left)/(right - left);

    M(1, 1) = 2*near/(bottom - top);
    M(1, 3) = -(top + bottom)/(bottom - top);

    M(2, 2) = -2*far*near/(far - near);
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
 * @return Matrix4f* on success, NULL otherwise.
 * */
Matrix4f* matrix_4f_look_at(
    Float32 eyeX, Float32 eyeY, Float32 eyeZ,
    Float32 targetX, Float32 targetY, Float32 targetZ,
    Float32 upX, Float32 upY, Float32 upZ
) {
    Matrix4f* p_mat = matrix_4f_identity();
    RETURN_VALUE_IF_FAIL(p_mat, NULL, "Failed to create LookAt matrix\n");

    Vector3f* eye = vector_3f_create(eyeX, eyeY, eyeZ);
    Vector3f* target = vector_3f_create(targetX, targetY, targetZ);
    Vector3f* up = vector_3f_create(upX, upY, upZ);

    Vector3f* fwd = vector_3f_sub(eye, target);
    Vector3f* right = vector_3f_cross(up, fwd);

    vector_3f_destroy(up);
    up = vector_3f_cross(fwd, right);

    vector_3f_normalize(fwd);
    vector_3f_normalize(right);

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

    vector_3f_destroy(eye);
    vector_3f_destroy(target);
    vector_3f_destroy(up);
    vector_3f_destroy(fwd);
    vector_3f_destroy(right);

    matrix_4f_translate(p_mat, eyeX, eyeY, eyeZ);

    return p_mat;
}

/**
 * Create translation matrix.
 *
 * @param dx Change in X coordinate
 * @param dy Change in Y coordinate
 * @param dz Change in Z coordinate
 *
 * @return Matrix4f* on success, NULL otherwise.
 * */
Matrix4f* matrix_4f_translation_matrix(Float32 dx, Float32 dy, Float32 dz) {
    Matrix4f* p_mat = matrix_4f_identity();
    RETURN_VALUE_IF_FAIL(p_mat, NULL, "Failed to create translation matrix\n");

#define M(r, c) p_mat->data[r][c]
    M(0, 3) = dx;
    M(1, 3) = dy;
    M(2, 3) = dz;
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
 * @return Matrix4f* on success, NULL otherwise.
 * */
Matrix4f* matrix_4f_rotation_matrix(Float32 yaw, Float32 pitch, Float32 roll) {
    Matrix4f* p_mat = matrix_4f_identity();
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
 * @return Matrix4f* on success, NULL otherwise.
 * */
Matrix4f* matrix_4f_scale_matrix(Float32 sx, Float32 sy, Float32 sz) {
    Matrix4f* p_mat = matrix_4f_identity();
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
void matrix_4f_translate(Matrix4f* p_mat, Float32 dx, Float32 dy, Float32 dz) {
    RETURN_IF_FAIL(p_mat, "Failed to create translation matrix\n");
    Matrix4f* p_trmat = matrix_4f_translation_matrix(dx, dy, dz);
    RETURN_IF_FAIL(p_trmat, "Failed to create translation matrix\n");
    matrix_4f_mul(p_mat, p_trmat);
    matrix_4f_destroy(p_trmat);
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
void matrix_4f_rotate(Matrix4f* p_mat, Float32 yaw, Float32 pitch, Float32 roll) {
    RETURN_IF_FAIL(p_mat, "Failed to create translation matrix\n");
    Matrix4f* p_rotmat = matrix_4f_rotation_matrix(yaw, pitch, roll);
    RETURN_IF_FAIL(p_rotmat, "Failed to create rotation matrix\n");
    matrix_4f_mul(p_mat, p_rotmat);
    matrix_4f_destroy(p_rotmat);
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
void matrix_4f_scale(Matrix4f* p_mat, Float32 sx, Float32 sy, Float32 sz) {
    RETURN_IF_FAIL(p_mat, "Failed to create translation matrix\n");
    Matrix4f* p_scalemat = matrix_4f_scale_matrix(sx, sy, sz);
    RETURN_IF_FAIL(p_scalemat, "Failed to create scale matrix\n");
    matrix_4f_mul(p_mat, p_scalemat);
    matrix_4f_destroy(p_scalemat);
}

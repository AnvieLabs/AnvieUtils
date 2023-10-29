#include <Anvie/Maths/Vector3f.h>
#include <Anvie/HelperDefines.h>
#include <string.h>
#include <math.h>

/**
 * Create a new Vector3f
 *
 * @return Vector3f* on success, NULL otherwise.
 * */
inline Vector3f* vector_3f_create(Float32 x, Float32 y, Float32 z) {
    Vector3f* p_vec = NEW(Vector3f);
    RETURN_VALUE_IF_FAIL(p_vec, NULL, ERR_OUT_OF_MEMORY);

    p_vec->x = x;
    p_vec->y = y;
    p_vec->z = z;

    return p_vec;
}

/**
 * Destroy given Vector3f
 *
 * @param p_vec
 * */
void vector_3f_destroy(Vector3f* p_vec) {
    RETURN_IF_FAIL(p_vec, ERR_INVALID_ARGUMENTS);
    FREE(p_vec);
}

/**
 * Create a copy of given vector.
 *
 * @param p_vec
 * @return Vector3f* on success, False otherwise.
 * */
inline Vector3f* vector_3f_create_copy(Vector3f* p_vec) {
    Vector3f* p_copy_vec = vector_3f_create(p_vec->x, p_vec->y, p_vec->z);
    RETURN_VALUE_IF_FAIL(p_copy_vec, NULL, "Failed to create copy of given vector\n");
    return p_copy_vec;
}

/**
 * Create Vector3f representing Origin.
 *
 * @return Vector3f* on success, NULL otherwise.
 * */
inline Vector3f* vector_3f_origin() {
    Vector3f* p_vec = vector_3f_create(0, 0, 0);
    RETURN_VALUE_IF_FAIL(p_vec, NULL, "Failed to create Origin Vector3f\n");
    return p_vec;
}

/**
 * Create Vector3f representing X Axis.
 *
 * @return Vector3f* on success, NULL otherwise.
 * */
Vector3f* vector_3f_x_axis() {
    Vector3f* p_vec = vector_3f_create(1, 0, 0);
    RETURN_VALUE_IF_FAIL(p_vec, NULL, "Failed to create X Axis Vector3f\n");
    return p_vec;
}

/**
 * Create Vector3f representing Y Axis.
 *
 * @return Vector3f* on success, NULL otherwise.
 * */
Vector3f* vector_3f_y_axis() {
    Vector3f* p_vec = vector_3f_create(0, 1, 0);
    RETURN_VALUE_IF_FAIL(p_vec, NULL, "Failed to create Y Axis Vector3f\n");
    return p_vec;
}

/**
 * Create Vector3f representing Z Axis.
 *
 * @return Vector3f* on success, NULL otherwise.
 * */
Vector3f* vector_3f_z_axis() {
    Vector3f* p_vec = vector_3f_create(0, 0, 1);
    RETURN_VALUE_IF_FAIL(p_vec, NULL, "Failed to create Z Axis Vector3f\n");
    return p_vec;
}

/**
 * Add first and second vector.
 * New returned vector must be destroyed after use.
 *
 * @param p_vec1
 * @param p_vec2
 * @return Vector3f* on success, NULL otherwise.
 * */
Vector3f* vector_3f_add(Vector3f* p_vec1, Vector3f* p_vec2) {
    RETURN_VALUE_IF_FAIL(p_vec1 && p_vec2, NULL, ERR_INVALID_ARGUMENTS);

    Vector3f* p_new_vec = vector_3f_origin();
    RETURN_VALUE_IF_FAIL(p_new_vec, NULL, "Failed to create new vector for storing result\n");

#define V1(t) p_vec1->t
#define V2(t) p_vec2->t
#define VN(t) p_new_vec->t

    VN(x) = V1(x) + V2(x);
    VN(y) = V1(y) + V2(y);
    VN(z) = V1(z) + V2(z);

#undef V1
#undef V2
#undef VN

    return p_new_vec;
}

/**
 * Subtract second from first vector.
 * New returned vector must be destroyed after use.
 *
 * @param p_vec1
 * @param p_vec2
 * @return Vector3f* on success, NULL otherwise.
 * */
Vector3f* vector_3f_sub(Vector3f* p_vec1, Vector3f* p_vec2) {
    RETURN_VALUE_IF_FAIL(p_vec1 && p_vec2, NULL, ERR_INVALID_ARGUMENTS);

    Vector3f* p_new_vec = vector_3f_origin();
    RETURN_VALUE_IF_FAIL(p_new_vec, NULL, "Failed to create new vector for storing result\n");

#define V1(t) p_vec1->t
#define V2(t) p_vec2->t
#define VN(t) p_new_vec->t

    VN(x) = V1(x) - V2(x);
    VN(y) = V1(y) - V2(y);
    VN(z) = V1(z) - V2(z);

#undef V1
#undef V2
#undef VN

    return p_new_vec;
}

/**
 * Scale (multiply) given vector with a scalar.
 * New returned vector must be destroyed after use.
 *
 * @param p_vec
 * @param scale
 * @return Vector3f* on success, NULL otherwise.
 * */
Vector3f* vector_3f_scale(Vector3f* p_vec, Float32 scale) {
    RETURN_VALUE_IF_FAIL(p_vec, NULL, ERR_INVALID_ARGUMENTS);

    Vector3f* p_new_vec = vector_3f_origin();
    RETURN_VALUE_IF_FAIL(p_new_vec, NULL, "Failed to create new vector for storing result\n");

#define V(t) p_vec->t
#define VN(t) p_new_vec->t

    VN(x) = V(x) * scale;
    VN(y) = V(y) * scale;
    VN(z) = V(z) * scale;

#undef V
#undef VN

    return p_new_vec;
}

/**
 * Compute dot product of two vectors and return result.
 *
 * @param p_vec1
 * @param p_vec2
 * @return Result of dot product.
 * */
Float32 vector_3f_dot(Vector3f* p_vec1, Vector3f* p_vec2) {
    RETURN_VALUE_IF_FAIL(p_vec1 && p_vec2, 0, ERR_INVALID_ARGUMENTS);

#define V1(t) p_vec1->t
#define V2(t) p_vec2->t

    return V1(x)*V2(x) + V1(y)*V2(y) + V1(z)*V2(z);

#undef V1
#undef V2
}

/**
 * Compute cross product of two vectors.
 * New returned vector must be destroyed after use.
 *
 * @param p_vec1
 * @param p_vec2
 * @return Vector3f* on success, NULL otherwise.
 * */
Vector3f* vector_3f_cross(Vector3f* p_vec1, Vector3f* p_vec2) {
    RETURN_VALUE_IF_FAIL(p_vec1 && p_vec2, NULL, ERR_INVALID_ARGUMENTS);

    Vector3f* p_new_vec = vector_3f_origin();
    RETURN_VALUE_IF_FAIL(p_new_vec, NULL, "Failed to create new vector for storing result\n");

#define V1(t) p_vec1->t
#define V2(t) p_vec2->t
#define VN(t) p_new_vec->t

    VN(x) = V1(y)*V2(z) - V1(z)*V2(y);
    VN(y) = V1(x)*V2(z) - V1(z)*V2(x);
    VN(z) = V1(y)*V2(x) - V1(x)*V2(y);

#undef V1
#undef V2
#undef VN

    return p_new_vec;
}

/**
 * Compute norm of given vector and return result,
 *
 * @param p_vec
 * @return Float32
 * */
inline Float32 vector_3f_compute_norm(Vector3f* p_vec) {
    RETURN_VALUE_IF_FAIL(p_vec, 0, ERR_INVALID_ARGUMENTS);

#define V(t) p_vec->t
    return sqrtf(V(x)*V(x) + V(y)*V(y) + V(z)*V(z));
#undef V
}

/**
 * Normalize given vector and store the result in it.
 *
 * @param p_vec
 * */
void vector_3f_normalize(Vector3f* p_vec) {
    RETURN_IF_FAIL(p_vec, ERR_INVALID_ARGUMENTS);

#define V(t) p_vec->t

    Float32 norm = vector_3f_compute_norm(p_vec);
    V(x) = V(x) / norm;
    V(y) = V(y) / norm;
    V(z) = V(z) / norm;

#undef V
}

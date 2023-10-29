#include <Anvie/Maths/Vector2f.h>
#include <Anvie/HelperDefines.h>
#include <string.h>
#include <math.h>

/**
 * Create a new Vector2f
 *
 * @return Vector2f* on success, NULL otherwise.
 * */
inline Vector2f* vector_2f_create(Float32 x, Float32 y) {
    Vector2f* p_vec = NEW(Vector2f);
    RETURN_VALUE_IF_FAIL(p_vec, NULL, ERR_OUT_OF_MEMORY);

    p_vec->x = x;
    p_vec->y = y;

    return p_vec;
}

/**
 * Destroy given Vector2f
 *
 * @param p_vec
 * */
void vector_2f_destroy(Vector2f* p_vec) {
    RETURN_IF_FAIL(p_vec, ERR_INVALID_ARGUMENTS);
    FREE(p_vec);
}

/**
 * Create a copy of given vector.
 *
 * @param p_vec
 * @return Vector2f* on success, False otherwise.
 * */
inline Vector2f* vector_2f_create_copy(Vector2f* p_vec) {
    Vector2f* p_copy_vec = vector_2f_create(p_vec->x, p_vec->y);
    RETURN_VALUE_IF_FAIL(p_copy_vec, NULL, "Failed to create copy of given vector\n");
    return p_copy_vec;
}

/**
 * Create Vector2f representing Origin.
 *
 * @return Vector2f* on success, NULL otherwise.
 * */
inline Vector2f* vector_2f_origin() {
    Vector2f* p_vec = vector_2f_create(0, 0);
    RETURN_VALUE_IF_FAIL(p_vec, NULL, "Failed to create Origin Vector2f\n");
    return p_vec;
}

/**
 * Create Vector2f representing X Axis.
 *
 * @return Vector2f* on success, NULL otherwise.
 * */
Vector2f* vector_2f_x_axis() {
    Vector2f* p_vec = vector_2f_create(1, 0);
    RETURN_VALUE_IF_FAIL(p_vec, NULL, "Failed to create X Axis Vector2f\n");
    return p_vec;
}

/**
 * Create Vector2f representing Y Axis.
 *
 * @return Vector2f* on success, NULL otherwise.
 * */
Vector2f* vector_2f_y_axis() {
    Vector2f* p_vec = vector_2f_create(0, 1);
    RETURN_VALUE_IF_FAIL(p_vec, NULL, "Failed to create Y Axis Vector2f\n");
    return p_vec;
}

/**
 * Add first and second vector.
 * New returned vector must be destroyed after use.
 *
 * @param p_vec1
 * @param p_vec2
 * @return Vector2f* on success, NULL otherwise.
 * */
Vector2f* vector_2f_add(Vector2f* p_vec1, Vector2f* p_vec2) {
    RETURN_VALUE_IF_FAIL(p_vec1 && p_vec2, NULL, ERR_INVALID_ARGUMENTS);

    Vector2f* p_new_vec = vector_2f_origin();
    RETURN_VALUE_IF_FAIL(p_new_vec, NULL, "Failed to create new vector for storing result\n");

#define V1(t) p_vec1->t
#define V2(t) p_vec2->t
#define VN(t) p_new_vec->t

    VN(x) = V1(x) + V2(x);
    VN(y) = V1(y) + V2(y);

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
 * @return Vector2f* on success, NULL otherwise.
 * */
Vector2f* vector_2f_sub(Vector2f* p_vec1, Vector2f* p_vec2) {
    RETURN_VALUE_IF_FAIL(p_vec1 && p_vec2, NULL, ERR_INVALID_ARGUMENTS);

    Vector2f* p_new_vec = vector_2f_origin();
    RETURN_VALUE_IF_FAIL(p_new_vec, NULL, "Failed to create new vector for storing result\n");

#define V1(t) p_vec1->t
#define V2(t) p_vec2->t
#define VN(t) p_new_vec->t

    VN(x) = V1(x) - V2(x);
    VN(y) = V1(y) - V2(y);

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
 * @return Vector2f* on success, NULL otherwise.
 * */
Vector2f* vector_2f_scale(Vector2f* p_vec, Float32 scale) {
    RETURN_VALUE_IF_FAIL(p_vec, NULL, ERR_INVALID_ARGUMENTS);

    Vector2f* p_new_vec = vector_2f_origin();
    RETURN_VALUE_IF_FAIL(p_new_vec, NULL, "Failed to create new vector for storing result\n");

#define V(t) p_vec->t
#define VN(t) p_new_vec->t

    VN(x) = V(x) * scale;
    VN(y) = V(y) * scale;

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
Float32 vector_2f_dot(Vector2f* p_vec1, Vector2f* p_vec2) {
    RETURN_VALUE_IF_FAIL(p_vec1 && p_vec2, 0, ERR_INVALID_ARGUMENTS);

#define V1(t) p_vec1->t
#define V2(t) p_vec2->t

    return V1(x)*V2(x) + V1(y)*V2(y);

#undef V1
#undef V2
}

/**
 * Compute norm of given vector and return result,
 *
 * @param p_vec
 * @return Float32
 * */
inline Float32 vector_2f_compute_norm(Vector2f* p_vec) {
    RETURN_VALUE_IF_FAIL(p_vec, 0, ERR_INVALID_ARGUMENTS);

#define V(t) p_vec->t
    return sqrtf(V(x)*V(x) + V(y)*V(y));
#undef V
}

/**
 * Normalize given vector and store the result in it.
 *
 * @param p_vec
 * */
void vector_2f_normalize(Vector2f* p_vec) {
    RETURN_IF_FAIL(p_vec, ERR_INVALID_ARGUMENTS);

#define V(t) p_vec->t

    Float32 norm = vector_2f_compute_norm(p_vec);
    V(x) = V(x) / norm;
    V(y) = V(y) / norm;

#undef V
}

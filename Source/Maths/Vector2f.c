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
    Vector2f* vec = NEW(Vector2f);
    RETURN_VALUE_IF_FAIL(vec, NULL, ERR_OUT_OF_MEMORY);

    vec->x = x;
    vec->y = y;

    return vec;
}

/**
 * Destroy given Vector2f
 *
 * @param vec
 * */
void vector_2f_destroy(Vector2f* vec) {
    RETURN_IF_FAIL(vec, ERR_INVALID_ARGUMENTS);
    FREE(vec);
}

/**
 * Create a copy of given vector.
 *
 * @param vec
 * @return Vector2f* on success, False otherwise.
 * */
inline Vector2f* vector_2f_create_copy(Vector2f* vec) {
    Vector2f* copy_vec = vector_2f_create(vec->x, vec->y);
    RETURN_VALUE_IF_FAIL(copy_vec, NULL, "Failed to create copy of given vector\n");
    return copy_vec;
}

/**
 * Create Vector2f representing Origin.
 *
 * @return Vector2f* on success, NULL otherwise.
 * */
inline Vector2f* vector_2f_origin() {
    Vector2f* vec = vector_2f_create(0, 0);
    RETURN_VALUE_IF_FAIL(vec, NULL, "Failed to create Origin Vector2f\n");
    return vec;
}

/**
 * Create Vector2f representing X Axis.
 *
 * @return Vector2f* on success, NULL otherwise.
 * */
Vector2f* vector_2f_x_axis() {
    Vector2f* vec = vector_2f_create(1, 0);
    RETURN_VALUE_IF_FAIL(vec, NULL, "Failed to create X Axis Vector2f\n");
    return vec;
}

/**
 * Create Vector2f representing Y Axis.
 *
 * @return Vector2f* on success, NULL otherwise.
 * */
Vector2f* vector_2f_y_axis() {
    Vector2f* vec = vector_2f_create(0, 1);
    RETURN_VALUE_IF_FAIL(vec, NULL, "Failed to create Y Axis Vector2f\n");
    return vec;
}

/**
 * Add first and second vector.
 * New returned vector must be destroyed after use.
 *
 * @param vec1
 * @param vec2
 * @return Vector2f* on success, NULL otherwise.
 * */
Vector2f* vector_2f_add(Vector2f* vec1, Vector2f* vec2) {
    RETURN_VALUE_IF_FAIL(vec1 && vec2, NULL, ERR_INVALID_ARGUMENTS);

    Vector2f* new_vec = vector_2f_origin();
    RETURN_VALUE_IF_FAIL(new_vec, NULL, "Failed to create new vector for storing result\n");

#define V1(t) vec1->t
#define V2(t) vec2->t
#define VN(t) new_vec->t

    VN(x) = V1(x) + V2(x);
    VN(y) = V1(y) + V2(y);

#undef V1
#undef V2
#undef VN

    return new_vec;
}

/**
 * Subtract second from first vector.
 * New returned vector must be destroyed after use.
 *
 * @param vec1
 * @param vec2
 * @return Vector2f* on success, NULL otherwise.
 * */
Vector2f* vector_2f_sub(Vector2f* vec1, Vector2f* vec2) {
    RETURN_VALUE_IF_FAIL(vec1 && vec2, NULL, ERR_INVALID_ARGUMENTS);

    Vector2f* new_vec = vector_2f_origin();
    RETURN_VALUE_IF_FAIL(new_vec, NULL, "Failed to create new vector for storing result\n");

#define V1(t) vec1->t
#define V2(t) vec2->t
#define VN(t) new_vec->t

    VN(x) = V1(x) - V2(x);
    VN(y) = V1(y) - V2(y);

#undef V1
#undef V2
#undef VN

    return new_vec;
}

/**
 * Scale (multiply) given vector with a scalar.
 * New returned vector must be destroyed after use.
 *
 * @param vec
 * @param scale
 * @return Vector2f* on success, NULL otherwise.
 * */
Vector2f* vector_2f_scale(Vector2f* vec, Float32 scale) {
    RETURN_VALUE_IF_FAIL(vec, NULL, ERR_INVALID_ARGUMENTS);

    Vector2f* new_vec = vector_2f_origin();
    RETURN_VALUE_IF_FAIL(new_vec, NULL, "Failed to create new vector for storing result\n");

#define V(t) vec->t
#define VN(t) new_vec->t

    VN(x) = V(x) * scale;
    VN(y) = V(y) * scale;

#undef V
#undef VN

    return new_vec;
}

/**
 * Compute dot product of two vectors and return result.
 *
 * @param vec1
 * @param vec2
 * @return Result of dot product.
 * */
Float32 vector_2f_dot(Vector2f* vec1, Vector2f* vec2) {
    RETURN_VALUE_IF_FAIL(vec1 && vec2, 0, ERR_INVALID_ARGUMENTS);

#define V1(t) vec1->t
#define V2(t) vec2->t

    return V1(x)*V2(x) + V1(y)*V2(y);

#undef V1
#undef V2
}

/**
 * Compute norm of given vector and return result,
 *
 * @param vec
 * @return Float32
 * */
inline Float32 vector_2f_compute_norm(Vector2f* vec) {
    RETURN_VALUE_IF_FAIL(vec, 0, ERR_INVALID_ARGUMENTS);

#define V(t) vec->t
    return sqrtf(V(x)*V(x) + V(y)*V(y));
#undef V
}

/**
 * Normalize given vector and store the result in it.
 *
 * @param vec
 * */
void vector_2f_normalize(Vector2f* vec) {
    RETURN_IF_FAIL(vec, ERR_INVALID_ARGUMENTS);

#define V(t) vec->t

    Float32 norm = vector_2f_compute_norm(vec);
    V(x) = V(x) / norm;
    V(y) = V(y) / norm;

#undef V
}

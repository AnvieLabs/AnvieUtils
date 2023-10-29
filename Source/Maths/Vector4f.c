#include <Anvie/Maths/Vector4f.h>
#include <Anvie/HelperDefines.h>
#include <string.h>
#include <math.h>

/**
 * Create a new Vector4f
 *
 * @return Vector4f* on success, NULL otherwise.
 * */
inline Vector4f* vector_4f_create(Float32 x, Float32 y, Float32 z, Float32 t) {
    Vector4f* vec = NEW(Vector4f);
    RETURN_VALUE_IF_FAIL(vec, NULL, ERR_OUT_OF_MEMORY);

    vec->x = x;
    vec->y = y;
    vec->z = z;
    vec->t = t;

    return vec;
}

/**
 * Destroy given Vector4f
 *
 * @param vec
 * */
void vector_4f_destroy(Vector4f* vec) {
    RETURN_IF_FAIL(vec, ERR_INVALID_ARGUMENTS);
    FREE(vec);
}

/**
 * Create a copy of given vector.
 *
 * @param vec
 * @return Vector4f* on success, False otherwise.
 * */
inline Vector4f* vector_4f_create_copy(Vector4f* vec) {
    Vector4f* copy_vec = vector_4f_create(vec->x, vec->y, vec->z, vec->t);
    RETURN_VALUE_IF_FAIL(copy_vec, NULL, "Failed to create copy of given vector\n");
    return copy_vec;
}

/**
 * Create Vector4f representing Origin.
 *
 * @return Vector4f* on success, NULL otherwise.
 * */
inline Vector4f* vector_4f_origin() {
    Vector4f* vec = vector_4f_create(0, 0, 0, 0);
    RETURN_VALUE_IF_FAIL(vec, NULL, "Failed to create Origin Vector4f\n");
    return vec;
}

/**
 * Create Vector4f representing X Axis.
 *
 * @return Vector4f* on success, NULL otherwise.
 * */
Vector4f* vector_4f_x_axis() {
    Vector4f* vec = vector_4f_create(1, 0, 0, 0);
    RETURN_VALUE_IF_FAIL(vec, NULL, "Failed to create X Axis Vector4f\n");
    return vec;
}

/**
 * Create Vector4f representing Y Axis.
 *
 * @return Vector4f* on success, NULL otherwise.
 * */
Vector4f* vector_4f_y_axis() {
    Vector4f* vec = vector_4f_create(0, 1, 0, 0);
    RETURN_VALUE_IF_FAIL(vec, NULL, "Failed to create Y Axis Vector4f\n");
    return vec;
}

/**
 * Create Vector4f representing Z Axis.
 *
 * @return Vector4f* on success, NULL otherwise.
 * */
Vector4f* vector_4f_z_axis() {
    Vector4f* vec = vector_4f_create(0, 0, 1, 0);
    RETURN_VALUE_IF_FAIL(vec, NULL, "Failed to create Z Axis Vector4f\n");
    return vec;
}

/**
 * Create Vector4f representing Z Axis.
 *
 * @return Vector4f* on success, NULL otherwise.
 * */
Vector4f* vector_4f_t_axis() {
    Vector4f* vec = vector_4f_create(0, 0, 0, 1);
    RETURN_VALUE_IF_FAIL(vec, NULL, "Failed to create T Axis Vector4f\n");
    return vec;
}

/**
 * Add first and second vector.
 * New returned vector must be destroyed after use.
 *
 * @param vec1
 * @param vec2
 * @return Vector4f* on success, NULL otherwise.
 * */
Vector4f* vector_4f_add(Vector4f* vec1, Vector4f* vec2) {
    RETURN_VALUE_IF_FAIL(vec1 && vec2, NULL, ERR_INVALID_ARGUMENTS);

    Vector4f* new_vec = vector_4f_origin();
    RETURN_VALUE_IF_FAIL(new_vec, NULL, "Failed to create new vector for storing result\n");

#define V1(k) vec1->k
#define V2(k) vec2->k
#define VN(k) new_vec->k

    VN(x) = V1(x) + V2(x);
    VN(y) = V1(y) + V2(y);
    VN(z) = V1(z) + V2(z);
    VN(t) = V1(t) + V2(t);

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
 * @return Vector4f* on success, NULL otherwise.
 * */
Vector4f* vector_4f_sub(Vector4f* vec1, Vector4f* vec2) {
    RETURN_VALUE_IF_FAIL(vec1 && vec2, NULL, ERR_INVALID_ARGUMENTS);

    Vector4f* new_vec = vector_4f_origin();
    RETURN_VALUE_IF_FAIL(new_vec, NULL, "Failed to create new vector for storing result\n");

#define V1(k) vec1->k
#define V2(k) vec2->k
#define VN(k) new_vec->k

    VN(x) = V1(x) - V2(x);
    VN(y) = V1(y) - V2(y);
    VN(z) = V1(z) - V2(z);
    VN(t) = V1(t) - V2(t);

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
 * @return Vector4f* on success, NULL otherwise.
 * */
Vector4f* vector_4f_scale(Vector4f* vec, Float32 scale) {
    RETURN_VALUE_IF_FAIL(vec, NULL, ERR_INVALID_ARGUMENTS);

    Vector4f* new_vec = vector_4f_origin();
    RETURN_VALUE_IF_FAIL(new_vec, NULL, "Failed to create new vector for storing result\n");

#define V(k) vec->k
#define VN(k) new_vec->k

    VN(x) = V(x) * scale;
    VN(y) = V(y) * scale;
    VN(z) = V(z) * scale;
    VN(t) = V(t) * scale;

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
Float32 vector_4f_dot(Vector4f* vec1, Vector4f* vec2) {
    RETURN_VALUE_IF_FAIL(vec1 && vec2, 0, ERR_INVALID_ARGUMENTS);

#define V1(k) vec1->k
#define V2(k) vec2->k

    return V1(x)*V2(x) + V1(y)*V2(y) + V1(z)*V2(z) + V1(t)*V2(t);

#undef V1
#undef V2
}

/**
 * Compute norm of given vector and return result,
 *
 * @param vec
 * @return Float32
 * */
inline Float32 vector_4f_compute_norm(Vector4f* vec) {
    RETURN_VALUE_IF_FAIL(vec, 0, ERR_INVALID_ARGUMENTS);

#define V(k) vec->k
    return sqrtf(V(x)*V(x) + V(y)*V(y) + V(z)*V(z) + V(t)*V(t));
#undef V
}

/**
 * Normalize given vector and store the result in it.
 *
 * @param vec
 * */
void vector_4f_normalize(Vector4f* vec) {
    RETURN_IF_FAIL(vec, ERR_INVALID_ARGUMENTS);

#define V(k) vec->k

    Float32 norm = vector_4f_compute_norm(vec);
    V(x) = V(x) / norm;
    V(y) = V(y) / norm;
    V(z) = V(z) / norm;
    V(t) = V(t) / norm;

#undef V
}

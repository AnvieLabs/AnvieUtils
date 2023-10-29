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
    Vector3f* vec = NEW(Vector3f);
    RETURN_VALUE_IF_FAIL(vec, NULL, ERR_OUT_OF_MEMORY);

    vec->x = x;
    vec->y = y;
    vec->z = z;

    return vec;
}

/**
 * Destroy given Vector3f
 *
 * @param vec
 * */
void vector_3f_destroy(Vector3f* vec) {
    RETURN_IF_FAIL(vec, ERR_INVALID_ARGUMENTS);
    FREE(vec);
}

/**
 * Create a copy of given vector.
 *
 * @param vec
 * @return Vector3f* on success, False otherwise.
 * */
inline Vector3f* vector_3f_create_copy(Vector3f* vec) {
    Vector3f* copy_vec = vector_3f_create(vec->x, vec->y, vec->z);
    RETURN_VALUE_IF_FAIL(copy_vec, NULL, "Failed to create copy of given vector\n");
    return copy_vec;
}

/**
 * Create Vector3f representing Origin.
 *
 * @return Vector3f* on success, NULL otherwise.
 * */
inline Vector3f* vector_3f_origin() {
    Vector3f* vec = vector_3f_create(0, 0, 0);
    RETURN_VALUE_IF_FAIL(vec, NULL, "Failed to create Origin Vector3f\n");
    return vec;
}

/**
 * Create Vector3f representing X Axis.
 *
 * @return Vector3f* on success, NULL otherwise.
 * */
Vector3f* vector_3f_x_axis() {
    Vector3f* vec = vector_3f_create(1, 0, 0);
    RETURN_VALUE_IF_FAIL(vec, NULL, "Failed to create X Axis Vector3f\n");
    return vec;
}

/**
 * Create Vector3f representing Y Axis.
 *
 * @return Vector3f* on success, NULL otherwise.
 * */
Vector3f* vector_3f_y_axis() {
    Vector3f* vec = vector_3f_create(0, 1, 0);
    RETURN_VALUE_IF_FAIL(vec, NULL, "Failed to create Y Axis Vector3f\n");
    return vec;
}

/**
 * Create Vector3f representing Z Axis.
 *
 * @return Vector3f* on success, NULL otherwise.
 * */
Vector3f* vector_3f_z_axis() {
    Vector3f* vec = vector_3f_create(0, 0, 1);
    RETURN_VALUE_IF_FAIL(vec, NULL, "Failed to create Z Axis Vector3f\n");
    return vec;
}

/**
 * Add first and second vector.
 * New returned vector must be destroyed after use.
 *
 * @param vec1
 * @param vec2
 * @return Vector3f* on success, NULL otherwise.
 * */
Vector3f* vector_3f_add(Vector3f* vec1, Vector3f* vec2) {
    RETURN_VALUE_IF_FAIL(vec1 && vec2, NULL, ERR_INVALID_ARGUMENTS);

    Vector3f* new_vec = vector_3f_origin();
    RETURN_VALUE_IF_FAIL(new_vec, NULL, "Failed to create new vector for storing result\n");

#define V1(t) vec1->t
#define V2(t) vec2->t
#define VN(t) new_vec->t

    VN(x) = V1(x) + V2(x);
    VN(y) = V1(y) + V2(y);
    VN(z) = V1(z) + V2(z);

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
 * @return Vector3f* on success, NULL otherwise.
 * */
Vector3f* vector_3f_sub(Vector3f* vec1, Vector3f* vec2) {
    RETURN_VALUE_IF_FAIL(vec1 && vec2, NULL, ERR_INVALID_ARGUMENTS);

    Vector3f* new_vec = vector_3f_origin();
    RETURN_VALUE_IF_FAIL(new_vec, NULL, "Failed to create new vector for storing result\n");

#define V1(t) vec1->t
#define V2(t) vec2->t
#define VN(t) new_vec->t

    VN(x) = V1(x) - V2(x);
    VN(y) = V1(y) - V2(y);
    VN(z) = V1(z) - V2(z);

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
 * @return Vector3f* on success, NULL otherwise.
 * */
Vector3f* vector_3f_scale(Vector3f* vec, Float32 scale) {
    RETURN_VALUE_IF_FAIL(vec, NULL, ERR_INVALID_ARGUMENTS);

    Vector3f* new_vec = vector_3f_origin();
    RETURN_VALUE_IF_FAIL(new_vec, NULL, "Failed to create new vector for storing result\n");

#define V(t) vec->t
#define VN(t) new_vec->t

    VN(x) = V(x) * scale;
    VN(y) = V(y) * scale;
    VN(z) = V(z) * scale;

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
Float32 vector_3f_dot(Vector3f* vec1, Vector3f* vec2) {
    RETURN_VALUE_IF_FAIL(vec1 && vec2, 0, ERR_INVALID_ARGUMENTS);

#define V1(t) vec1->t
#define V2(t) vec2->t

    return V1(x)*V2(x) + V1(y)*V2(y) + V1(z)*V2(z);

#undef V1
#undef V2
}

/**
 * Compute cross product of two vectors.
 * New returned vector must be destroyed after use.
 *
 * @param vec1
 * @param vec2
 * @return Vector3f* on success, NULL otherwise.
 * */
Vector3f* vector_3f_cross(Vector3f* vec1, Vector3f* vec2) {
    RETURN_VALUE_IF_FAIL(vec1 && vec2, NULL, ERR_INVALID_ARGUMENTS);

    Vector3f* new_vec = vector_3f_origin();
    RETURN_VALUE_IF_FAIL(new_vec, NULL, "Failed to create new vector for storing result\n");

#define V1(t) vec1->t
#define V2(t) vec2->t
#define VN(t) new_vec->t

    VN(x) = V1(y)*V2(z) - V1(z)*V2(y);
    VN(y) = V1(x)*V2(z) - V1(z)*V2(x);
    VN(z) = V1(y)*V2(x) - V1(x)*V2(y);

#undef V1
#undef V2
#undef VN

    return new_vec;
}

/**
 * Compute norm of given vector and return result,
 *
 * @param vec
 * @return Float32
 * */
inline Float32 vector_3f_compute_norm(Vector3f* vec) {
    RETURN_VALUE_IF_FAIL(vec, 0, ERR_INVALID_ARGUMENTS);

#define V(t) vec->t
    return sqrtf(V(x)*V(x) + V(y)*V(y) + V(z)*V(z));
#undef V
}

/**
 * Normalize given vector and store the result in it.
 *
 * @param vec
 * */
void vector_3f_normalize(Vector3f* vec) {
    RETURN_IF_FAIL(vec, ERR_INVALID_ARGUMENTS);

#define V(t) vec->t

    Float32 norm = vector_3f_compute_norm(vec);
    V(x) = V(x) / norm;
    V(y) = V(y) / norm;
    V(z) = V(z) / norm;

#undef V
}

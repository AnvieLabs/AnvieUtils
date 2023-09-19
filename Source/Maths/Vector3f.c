#include <Anvie/Maths/Vector3f.h>
#include <Anvie/HelperDefines.h>
#include <string.h>
#include <math.h>

/**
 * Create a new Vector3f
 *
 * @return AnvVector3f* on success, NULL otherwise.
 * */
inline AnvVector3f* anv_vector_3f_create(Float32 x, Float32 y, Float32 z) {
    AnvVector3f* p_vec = NEW(AnvVector3f);
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
void anv_vector_3f_destroy(AnvVector3f* p_vec) {
    RETURN_IF_FAIL(p_vec, ERR_INVALID_ARGUMENTS);
    FREE(p_vec);
}

/**
 * Create a copy of given vector.
 *
 * @param p_vec
 * @return AnvVector3f* on success, False otherwise.
 * */
inline AnvVector3f* anv_vector_3f_create_copy(AnvVector3f* p_vec) {
    AnvVector3f* p_copy_vec = anv_vector_3f_create(p_vec->x, p_vec->y, p_vec->z);
    RETURN_VALUE_IF_FAIL(p_copy_vec, NULL, "Failed to create copy of given vector\n");
    return p_copy_vec;
}

/**
 * Create Vector3f representing Origin.
 *
 * @return AnvVector3f* on success, NULL otherwise.
 * */
inline AnvVector3f* anv_vector_3f_origin() {
    AnvVector3f* p_vec = anv_vector_3f_create(0, 0, 0);
    RETURN_VALUE_IF_FAIL(p_vec, NULL, "Failed to create Origin Vector3f\n");
    return p_vec;
}

/**
 * Create Vector3f representing X Axis.
 *
 * @return AnvVector3f* on success, NULL otherwise.
 * */
AnvVector3f* anv_vector_3f_x_axis() {
    AnvVector3f* p_vec = anv_vector_3f_create(1, 0, 0);
    RETURN_VALUE_IF_FAIL(p_vec, NULL, "Failed to create X Axis Vector3f\n");
    return p_vec;
}

/**
 * Create Vector3f representing Y Axis.
 *
 * @return AnvVector3f* on success, NULL otherwise.
 * */
AnvVector3f* anv_vector_3f_y_axis() {
    AnvVector3f* p_vec = anv_vector_3f_create(0, 1, 0);
    RETURN_VALUE_IF_FAIL(p_vec, NULL, "Failed to create Y Axis Vector3f\n");
    return p_vec;
}

/**
 * Create Vector3f representing Z Axis.
 *
 * @return AnvVector3f* on success, NULL otherwise.
 * */
AnvVector3f* anv_vector_3f_z_axis() {
    AnvVector3f* p_vec = anv_vector_3f_create(0, 0, 1);
    RETURN_VALUE_IF_FAIL(p_vec, NULL, "Failed to create Z Axis Vector3f\n");
    return p_vec;
}

/**
 * Add first and second vector.
 * New returned vector must be destroyed after use.
 *
 * @param p_vec1
 * @param p_vec2
 * @return AnvVector3f* on success, NULL otherwise.
 * */
AnvVector3f* anv_vector_3f_add(AnvVector3f* p_vec1, AnvVector3f* p_vec2) {
    RETURN_VALUE_IF_FAIL(p_vec1 && p_vec2, NULL, ERR_INVALID_ARGUMENTS);

    AnvVector3f* p_new_vec = anv_vector_3f_origin();
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
 * @return AnvVector3f* on success, NULL otherwise.
 * */
AnvVector3f* anv_vector_3f_sub(AnvVector3f* p_vec1, AnvVector3f* p_vec2) {
    RETURN_VALUE_IF_FAIL(p_vec1 && p_vec2, NULL, ERR_INVALID_ARGUMENTS);

    AnvVector3f* p_new_vec = anv_vector_3f_origin();
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
 * @return AnvVector3f* on success, NULL otherwise.
 * */
AnvVector3f* anv_vector_3f_scale(AnvVector3f* p_vec, Float32 scale) {
    RETURN_VALUE_IF_FAIL(p_vec, NULL, ERR_INVALID_ARGUMENTS);

    AnvVector3f* p_new_vec = anv_vector_3f_origin();
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
Float32 anv_vector_3f_dot(AnvVector3f* p_vec1, AnvVector3f* p_vec2) {
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
 * @return AnvVector3f* on success, NULL otherwise.
 * */
AnvVector3f* anv_vector_3f_cross(AnvVector3f* p_vec1, AnvVector3f* p_vec2) {
    RETURN_VALUE_IF_FAIL(p_vec1 && p_vec2, NULL, ERR_INVALID_ARGUMENTS);

    AnvVector3f* p_new_vec = anv_vector_3f_origin();
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
inline Float32 anv_vector_3f_compute_norm(AnvVector3f* p_vec) {
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
void anv_vector_3f_normalize(AnvVector3f* p_vec) {
    RETURN_IF_FAIL(p_vec, ERR_INVALID_ARGUMENTS);

#define V(t) p_vec->t

    Float32 norm = anv_vector_3f_compute_norm(p_vec);
    V(x) = V(x) / norm;
    V(y) = V(y) / norm;
    V(z) = V(z) / norm;

#undef V
}

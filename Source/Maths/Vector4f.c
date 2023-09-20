#include <Anvie/Maths/Vector4f.h>
#include <Anvie/HelperDefines.h>
#include <string.h>
#include <math.h>

/**
 * Create a new Vector4f
 *
 * @return AnvVector4f* on success, NULL otherwise.
 * */
inline AnvVector4f* anv_vector_4f_create(Float32 x, Float32 y, Float32 z, Float32 t) {
    AnvVector4f* p_vec = NEW(AnvVector4f);
    RETURN_VALUE_IF_FAIL(p_vec, NULL, ERR_OUT_OF_MEMORY);

    p_vec->x = x;
    p_vec->y = y;
    p_vec->z = z;
    p_vec->t = t;

    return p_vec;
}

/**
 * Destroy given Vector4f
 *
 * @param p_vec
 * */
void anv_vector_4f_destroy(AnvVector4f* p_vec) {
    RETURN_IF_FAIL(p_vec, ERR_INVALID_ARGUMENTS);
    FREE(p_vec);
}

/**
 * Create a copy of given vector.
 *
 * @param p_vec
 * @return AnvVector4f* on success, False otherwise.
 * */
inline AnvVector4f* anv_vector_4f_create_copy(AnvVector4f* p_vec) {
    AnvVector4f* p_copy_vec = anv_vector_4f_create(p_vec->x, p_vec->y, p_vec->z, p_vec->t);
    RETURN_VALUE_IF_FAIL(p_copy_vec, NULL, "Failed to create copy of given vector\n");
    return p_copy_vec;
}

/**
 * Create Vector4f representing Origin.
 *
 * @return AnvVector4f* on success, NULL otherwise.
 * */
inline AnvVector4f* anv_vector_4f_origin() {
    AnvVector4f* p_vec = anv_vector_4f_create(0, 0, 0, 0);
    RETURN_VALUE_IF_FAIL(p_vec, NULL, "Failed to create Origin Vector4f\n");
    return p_vec;
}

/**
 * Create Vector4f representing X Axis.
 *
 * @return AnvVector4f* on success, NULL otherwise.
 * */
AnvVector4f* anv_vector_4f_x_axis() {
    AnvVector4f* p_vec = anv_vector_4f_create(1, 0, 0, 0);
    RETURN_VALUE_IF_FAIL(p_vec, NULL, "Failed to create X Axis Vector4f\n");
    return p_vec;
}

/**
 * Create Vector4f representing Y Axis.
 *
 * @return AnvVector4f* on success, NULL otherwise.
 * */
AnvVector4f* anv_vector_4f_y_axis() {
    AnvVector4f* p_vec = anv_vector_4f_create(0, 1, 0, 0);
    RETURN_VALUE_IF_FAIL(p_vec, NULL, "Failed to create Y Axis Vector4f\n");
    return p_vec;
}

/**
 * Create Vector4f representing Z Axis.
 *
 * @return AnvVector4f* on success, NULL otherwise.
 * */
AnvVector4f* anv_vector_4f_z_axis() {
    AnvVector4f* p_vec = anv_vector_4f_create(0, 0, 1, 0);
    RETURN_VALUE_IF_FAIL(p_vec, NULL, "Failed to create Z Axis Vector4f\n");
    return p_vec;
}

/**
 * Create Vector4f representing Z Axis.
 *
 * @return AnvVector4f* on success, NULL otherwise.
 * */
AnvVector4f* anv_vector_4f_t_axis() {
    AnvVector4f* p_vec = anv_vector_4f_create(0, 0, 0, 1);
    RETURN_VALUE_IF_FAIL(p_vec, NULL, "Failed to create T Axis Vector4f\n");
    return p_vec;
}

/**
 * Add first and second vector.
 * New returned vector must be destroyed after use.
 *
 * @param p_vec1
 * @param p_vec2
 * @return AnvVector4f* on success, NULL otherwise.
 * */
AnvVector4f* anv_vector_4f_add(AnvVector4f* p_vec1, AnvVector4f* p_vec2) {
    RETURN_VALUE_IF_FAIL(p_vec1 && p_vec2, NULL, ERR_INVALID_ARGUMENTS);

    AnvVector4f* p_new_vec = anv_vector_4f_origin();
    RETURN_VALUE_IF_FAIL(p_new_vec, NULL, "Failed to create new vector for storing result\n");

#define V1(k) p_vec1->k
#define V2(k) p_vec2->k
#define VN(k) p_new_vec->k

    VN(x) = V1(x) + V2(x);
    VN(y) = V1(y) + V2(y);
    VN(z) = V1(z) + V2(z);
    VN(t) = V1(t) + V2(t);

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
 * @return AnvVector4f* on success, NULL otherwise.
 * */
AnvVector4f* anv_vector_4f_sub(AnvVector4f* p_vec1, AnvVector4f* p_vec2) {
    RETURN_VALUE_IF_FAIL(p_vec1 && p_vec2, NULL, ERR_INVALID_ARGUMENTS);

    AnvVector4f* p_new_vec = anv_vector_4f_origin();
    RETURN_VALUE_IF_FAIL(p_new_vec, NULL, "Failed to create new vector for storing result\n");

#define V1(k) p_vec1->k
#define V2(k) p_vec2->k
#define VN(k) p_new_vec->k

    VN(x) = V1(x) - V2(x);
    VN(y) = V1(y) - V2(y);
    VN(z) = V1(z) - V2(z);
    VN(t) = V1(t) - V2(t);

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
 * @return AnvVector4f* on success, NULL otherwise.
 * */
AnvVector4f* anv_vector_4f_scale(AnvVector4f* p_vec, Float32 scale) {
    RETURN_VALUE_IF_FAIL(p_vec, NULL, ERR_INVALID_ARGUMENTS);

    AnvVector4f* p_new_vec = anv_vector_4f_origin();
    RETURN_VALUE_IF_FAIL(p_new_vec, NULL, "Failed to create new vector for storing result\n");

#define V(k) p_vec->k
#define VN(k) p_new_vec->k

    VN(x) = V(x) * scale;
    VN(y) = V(y) * scale;
    VN(z) = V(z) * scale;
    VN(t) = V(t) * scale;

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
Float32 anv_vector_4f_dot(AnvVector4f* p_vec1, AnvVector4f* p_vec2) {
    RETURN_VALUE_IF_FAIL(p_vec1 && p_vec2, 0, ERR_INVALID_ARGUMENTS);

#define V1(k) p_vec1->k
#define V2(k) p_vec2->k

    return V1(x)*V2(x) + V1(y)*V2(y) + V1(z)*V2(z) + V1(t)*V2(t);

#undef V1
#undef V2
}

/**
 * Compute norm of given vector and return result,
 *
 * @param p_vec
 * @return Float32
 * */
inline Float32 anv_vector_4f_compute_norm(AnvVector4f* p_vec) {
    RETURN_VALUE_IF_FAIL(p_vec, 0, ERR_INVALID_ARGUMENTS);

#define V(k) p_vec->k
    return sqrtf(V(x)*V(x) + V(y)*V(y) + V(z)*V(z) + V(t)*V(t));
#undef V
}

/**
 * Normalize given vector and store the result in it.
 *
 * @param p_vec
 * */
void anv_vector_4f_normalize(AnvVector4f* p_vec) {
    RETURN_IF_FAIL(p_vec, ERR_INVALID_ARGUMENTS);

#define V(k) p_vec->k

    Float32 norm = anv_vector_4f_compute_norm(p_vec);
    V(x) = V(x) / norm;
    V(y) = V(y) / norm;
    V(z) = V(z) / norm;
    V(t) = V(t) / norm;

#undef V
}

#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include "raylib.h"
#include "raymath.h"

inline Transform CustomMatrixToTransform(Matrix mat) {
    Transform transform;
    transform.translation = { mat.m12, mat.m13, mat.m14 };
    transform.rotation = QuaternionFromMatrix(mat);
    transform.scale.x = Vector3Length({ mat.m0, mat.m1, mat.m2 });
    transform.scale.y = Vector3Length({ mat.m4, mat.m5, mat.m6 });
    transform.scale.z = Vector3Length({ mat.m8, mat.m9, mat.m10 });
    return transform;
}

#endif
#ifndef KARBONATOR_GEOM3D_TRIANGLE_H
#define KARBONATOR_GEOM3D_TRIANGLE_H

#include "karbonator/pp/basic.h"
#include "karbonator/math/Vector3.h"

KARBONATOR_PP_C_LINKAGE_BEGIN()

typedef struct Triangle_ {
    union {
        Vector3 points [3];
        struct {
            Vector3 p0;
            Vector3 p1;
            Vector3 p2;
        } triplet;
    } m;
} Triangle;

KARBONATOR_PP_C_LINKAGE_END()

#endif

#ifndef KARBONATOR_GEOM3D_AABB_H
#define KARBONATOR_GEOM3D_AABB_H

#include "karbonator/pp/basic.h"
#include "karbonator/math/Vector3.h"

KARBONATOR_PP_C_LINKAGE_BEGIN()

typedef struct Aabb_ {
    Vector3 center;
    Vector3 extent;
} Aabb;

KARBONATOR_PP_C_LINKAGE_END()

#endif

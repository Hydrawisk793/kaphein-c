#ifndef KARBONATOR_GEOM3D_PLANE_H
#define KARBONATOR_GEOM3D_PLANE_H

#include "karbonator/pp/basic.h"
#include "karbonator/math/Vector3.h"

KARBONATOR_PP_C_LINKAGE_BEGIN()

typedef struct Plane_ {
    Vector3 center;
    Vector3 normal;
} Plane;

KARBONATOR_PP_C_LINKAGE_END()

#endif

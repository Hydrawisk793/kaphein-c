#ifndef KARBONATOR_GEOM3D_RAY_H
#define KARBONATOR_GEOM3D_RAY_H

#include "karbonator/pp/basic.h"
#include "karbonator/math/Vector3.h"

KARBONATOR_PP_C_LINKAGE_BEGIN()

typedef struct Ray_ {
    Vector3 point;
    Vector3 direction;
} Ray;

KARBONATOR_PP_C_LINKAGE_END()

#endif

#ifndef KARBONATOR_GEOM3D_SPHERE_H
#define KARBONATOR_GEOM3D_SPHERE_H

#include "karbonator/pp/basic.h"
#include "karbonator/math/Vector3.h"

KARBONATOR_PP_C_LINKAGE_BEGIN()

typedef struct Sphere_ {
    Vector3 center;
    float radius;
} Sphere;

KARBONATOR_PP_C_LINKAGE_END()

#endif

#ifndef KARBONATOR_GEOM3D_CAPSULE_H
#define KARBONATOR_GEOM3D_CAPSULE_H

#include "karbonator/pp/basic.h"
#include "karbonator/geom3d/LineSegment.h"

KARBONATOR_PP_C_LINKAGE_BEGIN()

typedef struct Capsule_ {
    LineSegment axis_;
    float radius_;
} Capsule;

KARBONATOR_PP_C_LINKAGE_END()

#endif

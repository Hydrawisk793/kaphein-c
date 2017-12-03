#ifndef KARBONATOR_GEOM3D_LINESEGMENT_H
#define KARBONATOR_GEOM3D_LINESEGMENT_H

#include "karbonator/pp/basic.h"
#include "karbonator/math/Vector3.h"

KARBONATOR_PP_C_LINKAGE_BEGIN()

typedef struct LineSegment_ {
    Vector3 startPoint;
    Vector3 endPoint;
} LineSegment;

KARBONATOR_PP_C_LINKAGE_END()

#endif

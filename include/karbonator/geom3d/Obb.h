#ifndef KARBONATOR_GEOM3D_OBB_H
#define KARBONATOR_GEOM3D_OBB_H

#include "karbonator/pp/basic.h"
#include "karbonator/math/Vector3.h"
#include "karbonator/math/Matrix3x3.h"

KARBONATOR_PP_C_LINKAGE_BEGIN()

typedef struct Obb_ {
        Vector3 center;
        Vector3 extent;
        Matrix3x3 axes;
} Obb;

KARBONATOR_PP_C_LINKAGE_END()

#endif

#ifndef KARBONATOR_NES_PPU_H
#define KARBONATOR_NES_PPU_H

#include <stdint.h>
#include "karbonator/karbonator.h"

KARBONATOR_BEGIN_C_LINKAGE

typedef struct Ppu_ {
    uint16_t addr;
} Ppu;

KARBONATOR_END_C_LINKAGE

#endif

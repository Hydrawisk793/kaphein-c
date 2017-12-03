#ifndef KARBONATOR_NES_APU_H
#define KARBONATOR_NES_APU_H

#include <stdint.h>

typedef struct Apu_ {
    uint8_t dummy_;
} Apu;

void Apu_run(
    Apu * pApu
);

#endif

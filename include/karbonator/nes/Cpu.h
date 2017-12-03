#ifndef KARBONATOR_NES_CPU_H
#define KARBONATOR_NES_CPU_H

#include <stdint.h>

typedef struct Cpu_ {
    struct {
        uint16_t pc;
        uint8_t p, s, a, x, y;
        uint8_t dummy_;
    } registers;

    uint8_t (*pReaderFunc) (void*, uint16_t*);
    void (*pWriterFunc) (void*, uint16_t*, uint8_t);
} Cpu;

void Cpu_run(
    Cpu * pContext
);

#endif

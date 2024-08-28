#pragma once
#include <stdlib.h>
#include <stdint.h>

typedef struct Naf_st {
    int32_t d[256];
    uint8_t top;
}NAF;

void initNaf(NAF* N, const int32_t* x, const size_t len);
#pragma once
#include <stdint.h>
#include "bn.h"

typedef struct ellipticCurve_st {
    BIGNUM* a;
    BIGNUM* b;
}ECURVE;

void initEcurve(ECURVE* EC, const BIGNUM* a, const BIGNUM* b);
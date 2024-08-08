#pragma once
#include "bn.h"

typedef struct bignumPoint_st {
    BIGNUM* x;
    BIGNUM* y;
}BIGNUMPOINT;

typedef struct bignumPointJaco_st {
    BIGNUM* x;
    BIGNUM* y;
    BIGNUM* z;
}BIGNUMPOINTJACO;

void initPoint(BIGNUMPOINT* BP, const BIGNUM* x, const BIGNUM* y);
void initPointJA(BIGNUMPOINTJACO* BP, const BIGNUM* x, const BIGNUM* y, const BIGNUM* z);
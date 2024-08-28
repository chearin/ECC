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

typedef struct bignumPointJaco2_st {
    BIGNUM x;
    BIGNUM y;
    BIGNUM z;
}BIGNUMPOINTJACO2;

void initPoint(BIGNUMPOINT* BP, const BIGNUM* x, const BIGNUM* y);
void initPointJA(BIGNUMPOINTJACO* BP, const BIGNUM* x, const BIGNUM* y, const BIGNUM* z);
void initPointJA2(BIGNUMPOINTJACO2* BP, const BIGNUM* x, const BIGNUM* y, const BIGNUM* z);
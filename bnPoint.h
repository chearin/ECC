#pragma once
#include "bn.h"

typedef struct bignumPoint_st {
    BIGNUM* x;
    BIGNUM* y;
}BIGNUMPOINT;

void initPoint(BIGNUMPOINT* BP, const BIGNUM* x, const BIGNUM* y);
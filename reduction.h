#pragma once
#include "bn.h"

void fastReduction(BIGNUM* r, const BIGNUM* a, const BIGNUM* P);
void fastReduction2(BIGNUM* r, const BIGNUM* a, const BIGNUM* P);

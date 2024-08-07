#pragma once
#include "bn.h"
#include "mul.h"

void karaAdd(BIGNUM* r, const BIGNUM* a, const BIGNUM* b, int* maxD, int depth);
void karaSub(BIGNUM* r, const BIGNUM* a, const BIGNUM* b, int* maxD, int depth);

void karaAddD2(BIGNUM* r, const BIGNUM* a, const BIGNUM* b, int* maxD, int depth);
void karaSubD2(BIGNUM* r, const BIGNUM* a, const BIGNUM* b, int* maxD, int depth);

void Bigkara(BIGNUM* r, const BIGNUM* a, const BIGNUM* b, int* maxD, int depth, int limit);
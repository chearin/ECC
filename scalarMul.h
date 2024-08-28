#pragma once
#include "bn.h"
#include "naf.h"
#include "affine.h"
#include "projective.h"

void LtRMul(BIGNUMPOINT* r, const BIGNUM* k, const BIGNUMPOINT* a, const BIGNUM* P);
void RtLMul(BIGNUMPOINT* r, const BIGNUM* k, const BIGNUMPOINT* a, const BIGNUM* P);
void Naf(NAF* r, const BIGNUM* scalar);
void windowFindOdd(uint8_t* u, uint8_t* t, const NAF* k, const uint8_t w, const uint8_t point);
void SlidingWindow(BIGNUMPOINT* r, const BIGNUM* scalar, const BIGNUMPOINT* a, const uint8_t w, const BIGNUM* P);
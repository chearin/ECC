#pragma once
#include "affine.h"
#include "projective.h"

void LtRMul(BIGNUMPOINT* r, const BIGNUM* k, const BIGNUMPOINT* a, const BIGNUM* P);
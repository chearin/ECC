#pragma once
#include <stdio.h>
#include <stdint.h>

typedef struct bignum_st {
    uint32_t d[64];
    int top; //실제 사용중인 d 배열의 크기
    uint8_t cb; //carry 또는 borrow
}BIGNUM;

void initBignum(const uint32_t* num, const size_t inlen, BIGNUM* BN);
uint32_t compare(const BIGNUM* a, const BIGNUM* b);

void BignumberAdd(BIGNUM* r, const BIGNUM* a, const BIGNUM* b);
void BignumberSub(BIGNUM* r, const BIGNUM* a, const BIGNUM* b);

void PF_addition(BIGNUM* c, const BIGNUM* P, const BIGNUM* a, const BIGNUM* b);
void PF_substraction(BIGNUM* c, const BIGNUM* P, const BIGNUM* a, const BIGNUM* b);

void BignumberLShift32(BIGNUM* r, const BIGNUM* a, const uint32_t n);
void BignumberLShift(BIGNUM* r, const BIGNUM* a, const uint32_t n);
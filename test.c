#define _CRT_SECURE_NO_WARNINGS
#include "test.h"

void AFFINE()
{
	FILE* fp0 = NULL;

	unsigned char input_str[1000] = { 0 };
	uint32_t P256[8] = { 0 };

	BIGNUM P = { 0, };
	BIGNUM ax = { 0, };
	BIGNUM ay = { 0, };
	BIGNUM bx = { 0, };
	BIGNUM by = { 0, };
	BIGNUM cx = { 0, };
	BIGNUM cy = { 0, };
	BIGNUM cur_a = { 0, };
	BIGNUM cur_b = { 0, };

	BIGNUMPOINT A = { 0, };
	BIGNUMPOINT B = { 0, };
	BIGNUMPOINT C = { 0, };

	ECURVE CUR1 = { 0, };
	size_t len = 0;

	fp0 = fileOpen("P256값.txt", "r");

	//p256값 저장
	fscanf(fp0, "%s", input_str);
	len = strlen(input_str) / (sizeof(int) * 2);
	str2hex(input_str, P256, len);
	initBignum(P256, len, &P);

	initPoint(&A, &ax, &ay);
	initPoint(&B, &bx, &by);
	initPoint(&C, &cx, &cy);
	initEcurve(&CUR1, &cur_a, &cur_b);

	//addition, doubling
	ECADD(&C, &A, &B, &P);
	ECDBL(&C, &CUR1, &A, &P);
}
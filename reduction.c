#include "reduction.h"

void fastReduction(BIGNUM* r, const BIGNUM* a, const BIGNUM* P)
{
	if (compare(a, P) == -1)
	{
		for (int i = 0; i < a->top; i++)
		{
			r->d[i] = a->d[i];
		}
		r->top = a->top;
		return;
	}
	BIGNUM s1 = { {a->d[0], a->d[1], a->d[2], a->d[3], a->d[4], a->d[5], a->d[6], a->d[7]}, 8, 0 };
	BIGNUM s2 = { {0, 0, 0, a->d[11], a->d[12], a->d[13], a->d[14], a->d[15]}, 8, 0 };
	BIGNUM s3 = { {0, 0, 0, a->d[12], a->d[13], a->d[14], a->d[15], 0}, 8, 0 };
	BIGNUM s4 = { {a->d[8], a->d[9], a->d[10],0, 0, 0, a->d[14], a->d[15]}, 8, 0 };
	BIGNUM s5 = { {a->d[9], a->d[10], a->d[11], a->d[13], a->d[14], a->d[15], a->d[13], a->d[8]}, 8, 0 };
	BIGNUM s6 = { {a->d[11], a->d[12], a->d[13], 0, 0, 0, a->d[8], a->d[10]}, 8, 0 };
	BIGNUM s7 = { {a->d[12], a->d[13], a->d[14], a->d[15], 0, 0, a->d[9], a->d[11]}, 8, 0 };
	BIGNUM s8 = { {a->d[13], a->d[14], a->d[15], a->d[8], a->d[9], a->d[10], 0, a->d[12]}, 8, 0 };
	BIGNUM s9 = { {a->d[14], a->d[15], 0, a->d[9], a->d[10], a->d[11], 0, a->d[13]}, 8, 0 };

	BIGNUM sum = { {0, }, 0, 0 };

	PF_addition(&sum, P, &s1, &s2);
	PF_addition(&sum, P, &sum, &s2);
	PF_addition(&sum, P, &sum, &s3);
	PF_addition(&sum, P, &sum, &s3);
	PF_addition(&sum, P, &sum, &s4);
	PF_addition(&sum, P, &sum, &s5);
	
	PF_substraction(&sum, P, &sum, &s6);
	PF_substraction(&sum, P, &sum, &s7);
	PF_substraction(&sum, P, &sum, &s8);
	PF_substraction(&sum, P, &sum, &s9);

	//계산결과
	for (int i = 0; i < sum.top; i++)
	{
		r->d[i] = sum.d[i];
	}
	for (int i = sum.top; i < 16; i++)
	{
		r->d[i] = 0;
	}
	r->top = sum.top;
}

void fastReduction2(BIGNUM* r, const BIGNUM* a, const BIGNUM* P)
{
	if (compare(a, P) == -1)
	{
		for (int i = 0; i < a->top; i++)
		{
			r->d[i] = a->d[i];
		}
		r->top = a->top;
		return;
	}
	BIGNUM s1 = { {a->d[0], a->d[1], a->d[2], a->d[3], a->d[4], a->d[5], a->d[6], a->d[7]}, 8, 0 };
	BIGNUM s2 = { {0, 0, 0, a->d[11], a->d[12], a->d[13], a->d[14], a->d[15]}, 8, 0 };
	BIGNUM s3 = { {0, 0, 0, a->d[12], a->d[13], a->d[14], a->d[15], 0}, 8, 0 };
	BIGNUM s4 = { {a->d[8], a->d[9], a->d[10],0, 0, 0, a->d[14], a->d[15]}, 8, 0 };
	BIGNUM s5 = { {a->d[9], a->d[10], a->d[11], a->d[13], a->d[14], a->d[15], a->d[13], a->d[8]}, 8, 0 };
	BIGNUM s6 = { {a->d[11], a->d[12], a->d[13], 0, 0, 0, a->d[8], a->d[10]}, 8, 0 };
	BIGNUM s7 = { {a->d[12], a->d[13], a->d[14], a->d[15], 0, 0, a->d[9], a->d[11]}, 8, 0 };
	BIGNUM s8 = { {a->d[13], a->d[14], a->d[15], a->d[8], a->d[9], a->d[10], 0, a->d[12]}, 8, 0 };
	BIGNUM s9 = { {a->d[14], a->d[15], 0, a->d[9], a->d[10], a->d[11], 0, a->d[13]}, 8, 0 };

	BIGNUM sum = { 0, };
	int8_t carry = 0;

	carry += (s2.d[s2.top - 1] & 0x80000000) >> 31;
	BignumberLShift(&s2, &s2, 1);
	s2.top = 8;
	BignumberAdd(&sum, &s1, &s2);
	carry += sum.cb;
	carry += (s3.d[s3.top - 1] & 0x80000000) >> 31;
	BignumberLShift(&s3, &s3, 1);
	s3.top = 8;
	BignumberAdd(&sum, &sum, &s3);
	carry += sum.cb;
	BignumberAdd(&sum, &sum, &s4);
	carry += sum.cb;
	BignumberAdd(&sum, &sum, &s5);
	carry += sum.cb;

	BignumberSub(&sum, &sum, &s6);
	carry -= sum.cb;
	BignumberSub(&sum, &sum, &s7);
	carry -= sum.cb;
	BignumberSub(&sum, &sum, &s8);
	carry -= sum.cb;
	BignumberSub(&sum, &sum, &s9);
	carry -= sum.cb;

	//한번에 mod P256
	if (carry < 0)
	{
		carry = -carry;
		for (int i = 0; i < carry; i++)
		{
			BignumberAdd(&sum, &sum, P);
		}
	}
	else
	{
		for (int i = 0; i < carry; i++)
		{
			BignumberSub(&sum, &sum, P);
		}
	}

	//계산결과
	for (int i = 0; i < sum.top; i++)
	{
		r->d[i] = sum.d[i];
	}
	for (int i = sum.top; i < 16; i++)
	{
		r->d[i] = 0;
	}
	r->top = sum.top;
}
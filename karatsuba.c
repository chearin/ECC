#include "karatsuba.h"

void karaAdd(BIGNUM* r, const BIGNUM* a, const BIGNUM* b, int* maxD, int depth)
{
	BIGNUM R0 = { {0,}, 0, 0 };
	BIGNUM R1 = { {0,}, 0, 0 };
	BIGNUM R2 = { {0,}, 0, 0 };

	BIGNUM a0 = { {0,}, 0, 0 };
	BIGNUM a1 = { {0,}, 0, 0 };
	BIGNUM asum = { {0,}, 0, 0 };

	BIGNUM b0 = { {0,}, 0, 0 };
	BIGNUM b1 = { {0,}, 0, 0 };
	BIGNUM bsum = { {0,}, 0, 0 };

	uint32_t AB[2] = { 0, };
	
	if (depth > *maxD)
	{
		*maxD = depth;
	}
	
	int l = a->top;

	if (l < b->top)
	{
		l = b->top;
	}

	//base case
	if (l == 1)
	{
		divisionMul(AB, a->d[0], b->d[0]);
		r->d[0] = AB[0];
		r->d[1] = AB[1];
		r->top = 2;
		for (int i = 0; i < 2; i++)
		{
			if (r->d[i] == 0)
			{
				r->top--;
			}
			else
			{
				break;
			}
		}
		return;
	}
	
	//recursive case
	//a0, a1, b0, b1 나누기
	for (int i = 0; i < l / 2; i++)
	{
		a0.d[i] = a->d[i];
		a0.top++;
		b0.d[i] = b->d[i];
		b0.top++;
	}
	for (int i = l / 2; i < l; i++)
	{	
		a1.d[i - l / 2] = a->d[i];
		a1.top++;
		b1.d[i - l / 2] = b->d[i];
		b1.top++;
	}
	//asum = a1 + a0, bsum = b1 + b0
	BignumberAdd(&asum, &a0, &a1);
	if (asum.cb)
	{
		asum.d[asum.top] = 1;
		asum.top++;
	}
	BignumberAdd(&bsum, &b0, &b1);
	if (bsum.cb)
	{
		bsum.d[bsum.top] = 1;
		bsum.top++;
	}

	//R2 = a1 * b1
	karaAdd(&R2, &a1, &b1, maxD, depth + 1);
	//R0 = a0 * b0
	karaAdd(&R0, &a0, &b0, maxD, depth + 1);
	//R1 = asum * bsum - R2 - R0
	karaAdd(&R1, &asum, &bsum, maxD, depth + 1);
	BignumberSub(&R1, &R1, &R2);
	BignumberSub(&R1, &R1, &R0);

	//R2*2^2l
	for (int i = R2.top - 1; i >= 0; i--)
	{
		R2.d[i + a0.top * 2] = R2.d[i];
		R2.d[i] = 0;
	}
	if (R2.top)
	{
		R2.top += (a0.top * 2);
	}	

	//R1*2^l
	for (int i = R1.top - 1; i >= 0; i--)
	{
		R1.d[i + a0.top] = R1.d[i];
		R1.d[i] = 0;
	}
	if (R1.top)
	{
		R1.top += a0.top;
	}	

	//R2*2^2l + R1*2^l + R0
	BignumberAdd(r, &R2, &R1);
	if (r->cb)
	{
		r->d[r->top] = 1;
		r->top++;
	}
	BignumberAdd(r, r, &R0);
	if (r->cb)
	{
		r->d[r->top] = 1;
		r->top++;
	}
}

void karaSub(BIGNUM* r, const BIGNUM* a, const BIGNUM* b, int* maxD, int depth)
{
	BIGNUM R0 = { {0,}, 0, 0 };
	BIGNUM R1 = { {0,}, 0, 0 };
	BIGNUM R2 = { {0,}, 0, 0 };
	BIGNUM tmp = { {0,}, 0, 0 };

	BIGNUM a0 = { {0,}, 0, 0 };
	BIGNUM a1 = { {0,}, 0, 0 };
	BIGNUM asub = { {0,}, 0, 0 };

	BIGNUM b0 = { {0,}, 0, 0 };
	BIGNUM b1 = { {0,}, 0, 0 };
	BIGNUM bsub = { {0,}, 0, 0 };

	uint32_t AB[2] = { 0, };
	uint8_t t = 0;

	if (depth > *maxD)
	{
		*maxD = depth;
	}

	int l = a->top;

	if (l < b->top)
	{
		l = b->top;
	}

	//base case
	if (l == 1)
	{
		divisionMul(AB, a->d[0], b->d[0]);
		r->d[0] = AB[0];
		r->d[1] = AB[1];
		r->top = 2;
		for (int i = 0; i < 2; i++)
		{
			if (r->d[i] == 0)
			{
				r->top--;
			}
			else
			{
				break;
			}
		}
		return;
	}

	//recursive case
	//a0, a1, b0, b1 나누기
	for (int i = 0; i < l / 2; i++)
	{
		a0.d[i] = a->d[i];
		a0.top++;
		b0.d[i] = b->d[i];
		b0.top++;
	}
	for (int i = l / 2; i < l; i++)
	{
		a1.d[i - l / 2] = a->d[i];
		a1.top++;
		b1.d[i - l / 2] = b->d[i];
		b1.top++;
	}
	//asub = |a1 - a0|, bsub = |b1 - b0|
	BignumberSub(&asub, &a1, &a0);
	if (asub.cb)
	{
		BignumberSub(&asub, &a0, &a1);
		t ^= 1;
	}
	BignumberSub(&bsub, &b1, &b0);
	if (bsub.cb)
	{
		BignumberSub(&bsub, &b0, &b1);
		t ^= 1;
	}

	//R2 = a1 * b1
	karaSub(&R2, &a1, &b1, maxD, depth + 1);
	//R0 = a0 * b0
	karaSub(&R0, &a0, &b0, maxD, depth + 1);
	//R1 = R2 + R0 - (-1)^t * (asub * bsub)
	karaSub(&R1, &asub, &bsub, maxD, depth + 1);
	BignumberAdd(&tmp, &R2, &R0);
	if (tmp.cb)
	{
		tmp.d[tmp.top] = 1;
		tmp.top++;
	}
	if (t)
	{
		BignumberAdd(&R1, &tmp, &R1);
		if (R1.cb)
		{
			R1.d[R1.top] = 1;
			R1.top++;
		}
	}
	else
	{
		BignumberSub(&R1, &tmp, &R1);
	}

	//R2*2^2l
	for (int i = R2.top - 1; i >= 0; i--)
	{
		R2.d[i + a0.top * 2] = R2.d[i];
		R2.d[i] = 0;
	}
	if (R2.top)
	{
		R2.top += (a0.top * 2);
	}

	//R1*2^l
	for (int i = R1.top - 1; i >= 0; i--)
	{
		R1.d[i + a0.top] = R1.d[i];
		R1.d[i] = 0;
	}
	if (R1.top)
	{
		R1.top += a0.top;
	}

	//R2*2^2l + R1*2^l + R0
	BignumberAdd(r, &R2, &R1);
	if (r->cb)
	{
		r->d[r->top] = 1;
		r->top++;
	}
	BignumberAdd(r, r, &R0);
	if (r->cb)
	{
		r->d[r->top] = 1;
		r->top++;
	}
}

void karaAddD2(BIGNUM* r, const BIGNUM* a, const BIGNUM* b, int* maxD, int depth)
{
	BIGNUM R0 = { {0,}, 0, 0 };
	BIGNUM R1 = { {0,}, 0, 0 };
	BIGNUM R2 = { {0,}, 0, 0 };

	BIGNUM a0 = { {0,}, 0, 0 };
	BIGNUM a1 = { {0,}, 0, 0 };
	BIGNUM asum = { {0,}, 0, 0 };

	BIGNUM b0 = { {0,}, 0, 0 };
	BIGNUM b1 = { {0,}, 0, 0 };
	BIGNUM bsum = { {0,}, 0, 0 };

	int l = a->top;

	if (depth > *maxD)
	{
		*maxD = depth;
	}

	if (l < b->top)
	{
		l = b->top;
	}

	//base case
	if (depth == 2)
	{
		OperandScanning(r, a, b);
		return;
	}

	//recursive case
	//a0, a1, b0, b1 나누기
	for (int i = 0; i < l / 2; i++)
	{
		a0.d[i] = a->d[i];
		a0.top++;
		b0.d[i] = b->d[i];
		b0.top++;
	}
	for (int i = l / 2; i < l; i++)
	{
		a1.d[i - l / 2] = a->d[i];
		a1.top++;
		b1.d[i - l / 2] = b->d[i];
		b1.top++;
	}
	//asum = a1 + a0, bsum = b1 + b0
	BignumberAdd(&asum, &a0, &a1);
	if (asum.cb)
	{
		asum.d[asum.top] = 1;
		asum.top++;
	}
	BignumberAdd(&bsum, &b0, &b1);
	if (bsum.cb)
	{
		bsum.d[bsum.top] = 1;
		bsum.top++;
	}

	//R2 = a1 * b1
	karaAddD2(&R2, &a1, &b1, maxD, depth + 1);
	//R0 = a0 * b0
	karaAddD2(&R0, &a0, &b0, maxD, depth + 1);
	//R1 = asum * bsum - R2 - R0
	karaAddD2(&R1, &asum, &bsum, maxD, depth + 1);
	BignumberSub(&R1, &R1, &R2);
	BignumberSub(&R1, &R1, &R0);

	//R2*2^2l
	for (int i = R2.top - 1; i >= 0; i--)
	{
		R2.d[i + a0.top * 2] = R2.d[i];
		R2.d[i] = 0;
	}
	if (R2.top)
	{
		R2.top += (a0.top * 2);
	}

	//R1*2^l
	for (int i = R1.top - 1; i >= 0; i--)
	{
		R1.d[i + a0.top] = R1.d[i];
		R1.d[i] = 0;
	}
	if (R1.top)
	{
		R1.top += a0.top;
	}

	//R2*2^2l + R1*2^l + R0
	BignumberAdd(r, &R2, &R1);
	if (r->cb)
	{
		r->d[r->top] = 1;
		r->top++;
	}
	BignumberAdd(r, r, &R0);
	if (r->cb)
	{
		r->d[r->top] = 1;
		r->top++;
	}
}

void karaSubD2(BIGNUM* r, const BIGNUM* a, const BIGNUM* b, int* maxD, int depth)
{
	BIGNUM R0 = { {0,}, 0, 0 };
	BIGNUM R1 = { {0,}, 0, 0 };
	BIGNUM R2 = { {0,}, 0, 0 };
	BIGNUM tmp = { {0,}, 0, 0 };

	BIGNUM a0 = { {0,}, 0, 0 };
	BIGNUM a1 = { {0,}, 0, 0 };
	BIGNUM asub = { {0,}, 0, 0 };

	BIGNUM b0 = { {0,}, 0, 0 };
	BIGNUM b1 = { {0,}, 0, 0 };
	BIGNUM bsub = { {0,}, 0, 0 };

	uint8_t t = 0;

	int l = a->top;

	if (depth > *maxD)
	{
		*maxD = depth;
	}

	if (l < b->top)
	{
		l = b->top;
	}

	//base case
	if (depth == 2)
	{
		OperandScanning(r, a, b);
		return;
	}

	//recursive case
	//a0, a1, b0, b1 나누기
	for (int i = 0; i < l / 2; i++)
	{
		a0.d[i] = a->d[i];
		a0.top++;
		b0.d[i] = b->d[i];
		b0.top++;
	}
	for (int i = l / 2; i < l; i++)
	{
		a1.d[i - l / 2] = a->d[i];
		a1.top++;
		b1.d[i - l / 2] = b->d[i];
		b1.top++;
	}
	//asub = |a1 - a0|, bsub = |b1 - b0|
	BignumberSub(&asub, &a1, &a0);
	if (asub.cb)
	{
		BignumberSub(&asub, &a0, &a1);
		t ^= 1;
	}
	BignumberSub(&bsub, &b1, &b0);
	if (bsub.cb)
	{
		BignumberSub(&bsub, &b0, &b1);
		t ^= 1;
	}

	//R2 = a1 * b1
	karaSubD2(&R2, &a1, &b1, maxD, depth + 1);
	//R0 = a0 * b0
	karaSubD2(&R0, &a0, &b0, maxD, depth + 1);
	//R1 = R2 + R0 - (-1)^t * (asub * bsub)
	karaSubD2(&R1, &asub, &bsub, maxD, depth + 1);
	BignumberAdd(&tmp, &R2, &R0);
	if (tmp.cb)
	{
		tmp.d[tmp.top] = 1;
		tmp.top++;
	}
	if (t)
	{
		BignumberAdd(&R1, &tmp, &R1);
		if (R1.cb)
		{
			R1.d[R1.top] = 1;
			R1.top++;
		}
	}
	else
	{
		BignumberSub(&R1, &tmp, &R1);
	}

	//R2*2^2l
	for (int i = R2.top - 1; i >= 0; i--)
	{
		R2.d[i + a0.top * 2] = R2.d[i];
		R2.d[i] = 0;
	}
	if (R2.top)
	{
		R2.top += (a0.top * 2);
	}

	//R1*2^l
	for (int i = R1.top - 1; i >= 0; i--)
	{
		R1.d[i + a0.top] = R1.d[i];
		R1.d[i] = 0;
	}
	if (R1.top)
	{
		R1.top += a0.top;
	}

	//R2*2^2l + R1*2^l + R0
	BignumberAdd(r, &R2, &R1);
	if (r->cb)
	{
		r->d[r->top] = 1;
		r->top++;
	}
	BignumberAdd(r, r, &R0);
	if (r->cb)
	{
		r->d[r->top] = 1;
		r->top++;
	}
}
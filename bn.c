#define _CRT_SECURE_NO_WARNINGS
#include "bn.h"

void initBignum(const uint32_t* num, const size_t inlen, BIGNUM* BN)
{
	for (int i = 0; i < inlen; i++)
	{
		BN->d[i] = num[i];
	}
	for (int i = inlen; i < 16; i++)
	{
		BN->d[i] = 0;
	}
	BN->top = inlen;
	BN->cb = 0;
}

//a > b 이면 1, a == b 이면 0, a < b 이면 -1 반환
uint32_t compare(const BIGNUM* a, const BIGNUM* b)
{
	if (a->top == b->top)
	{
		for (int i = 0; i < a->top; i++)
		{
			if (a->d[a->top - 1 - i] > b->d[a->top - 1 - i])
			{
				return 1;
			}
			else if (a->d[a->top - 1 - i] < b->d[a->top - 1 - i])
			{
				return -1;
			}
		}
		return 0;
	}
	else
	{
		return -1 + 2 * (a->top > b->top);
	}
}

void BignumberAdd(BIGNUM* r, const BIGNUM* a, const BIGNUM* b)
{
	uint32_t carry = 0;
	uint32_t result = 0;

	//a, b를 긴 길이인 것과 짧은 길이인 것으로 구분
	BIGNUM* l = a;
	BIGNUM* s = b;

	if (l->top < s->top)
	{
		l = b;
		s = a;
	}

	for (int i = 0; i < s->top; i++)
	{
		result = l->d[i] + s->d[i];
		if (result < l->d[i])
		{
			result += carry;
			carry = 1;
		}
		else
		{
			result += carry;
			if (result < carry)
			{
				carry = 1;
			}
			else
			{
				carry = 0;
			}
		}
		r->d[i] = result;
	}
	for (int i = s->top; i < l->top; i++)
	{
		result = l->d[i] + carry;
		carry = (result < l->d[i]);
		r->d[i] = result;
	}

	r->cb = carry;
	r->top = l->top;
}

void BignumberSub(BIGNUM* r, const BIGNUM* a, const BIGNUM* b)
{
	uint32_t borrow = 0;
	uint32_t result = 0;
	size_t len = a->top;

	// len은 a, b 중 긴쪽
	if (a->top < b->top)
	{
		len = b->top;
	}

	for (int i = 0; i < len; i++)
	{
		result = (a->d[i] - b->d[i] - borrow);

		// a->d[i] == b->d[i] 일때는 borrow 유지
		if (a->d[i] < b->d[i])
		{
			borrow = 1;
		}
		else if (a->d[i] > b->d[i])
		{
			borrow = 0;
		}
		r->d[i] = result;
	}
	r->cb = borrow;
	r->top = len;

	// 결과의 앞자리부터 0인지를 확인하고 길이를 줄이는 함수
	for (int i = r->top - 1; i >= 0; i--)
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
}

void PF_addition(BIGNUM* c, const BIGNUM* P, const BIGNUM* a, const BIGNUM* b)
{
	uint32_t cmp = 0;

	BignumberAdd(c, a, b);
	cmp = compare(c, P);
	if (c->cb || cmp == 0 || cmp == 1)
	{
		BignumberSub(c, c, P);
	}
}

void PF_substraction(BIGNUM* c, const BIGNUM* P, const BIGNUM* a, const BIGNUM* b)
{
	BignumberSub(c, a, b);
	if (c->cb)
	{
		BignumberAdd(c, c, P);
	}
}

void BignumberLShift32(BIGNUM* r, const BIGNUM* a, const uint32_t n)
{
	for (int i = 0; i < a->top; i++)
	{
		r->d[i + n] = a->d[i];
	}
	for (int i = 0; i < n; i++)
	{
		r->d[i] = 0;
	}
	r->top = a->top + n;
}

void BignumberLShift(BIGNUM* r, const BIGNUM* a, const uint32_t n)
{
	uint32_t q = n / 32;
	uint32_t rest = n % 32;
	uint32_t atop = a->top;

	BignumberLShift32(r, a, q);
	if (rest)
	{
		for (int i = r->top; i > 0; i--)
		{
			r->d[i] = r->d[i] << rest;
			r->d[i] += r->d[i - 1] >> (32 - rest);
		}
		r->d[0] <<= rest;
		r->top = atop + q + 1;
	}
}
#define _CRT_SECURE_NO_WARNINGS
#include "mul.h"

//분할 곱셈
void divisionMul(uint32_t* c, const uint32_t a, const uint32_t b)
{
	uint32_t carry = 0;
	uint32_t sum = 0;
	uint32_t r = 0;
	uint32_t t1 = 0, t2 = 0, t3 = 0;

	t1 = (a & MASK) * (b & MASK);
	r = (t1 & MASK);
	t2 = (a >> 16) * (b & MASK);
	t3 = (a & MASK) * (b >> 16);
	sum = (t1 >> 16) + (t2 & MASK) + (t3 & MASK);
	carry = (sum >> 16);
	r += (sum << 16);
	c[0] = r;

	t1 = (a >> 16) * (b >> 16);
	sum = (t2 >> 16) + (t3 >> 16) + (t1 & MASK) + carry;
	carry = (sum >> 16);
	r = (sum & MASK);
	sum = (t1 >> 16) + carry;
	r += (sum << 16);
	c[1] = r;
}

void OperandScanning(BIGNUM* r, const BIGNUM* a, const BIGNUM* b)
{
	uint32_t AB[2] = { 0, };
	uint32_t UV[2] = { 0, };
	uint32_t sum = 0;
	uint32_t carry = 0;

	BIGNUM tmpA = { 0, };
	BIGNUM tmpB = { 0, };

	for (int i = 0; i < a->top; i++)
	{
		tmpA.d[i] = a->d[i];
	}
	for (int i = 0; i < b->top; i++)
	{
		tmpB.d[i] = b->d[i];
	}	
	tmpA.top = a->top;
	tmpB.top = b->top;

	for (int i = 0; i < r->top; i++)
	{
		r->d[i] = 0;
	}

	for (int i = 0; i < tmpA.top; i++)
	{
		UV[1] = 0;
		for (int j = 0; j < tmpB.top; j++)
		{
			divisionMul(AB, tmpA.d[i], tmpB.d[j]);
			//캐리 확인
			carry = 0;
			sum = r->d[i + j] + AB[0];
			if (sum < r->d[i + j])
			{
				carry = 1;
				
			}
			sum += UV[1];
			if (sum < UV[1])
			{
				carry += 1;
			}

			UV[0] = sum;
			UV[1] = AB[1] + carry;
			r->d[i + j] = UV[0];
		}
		r->d[i + tmpB.top] = UV[1];
	}
	r->top = tmpA.top + tmpB.top;
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

void ProductScanning(BIGNUM* r, const BIGNUM* a, const BIGNUM* b)
{
	uint32_t UV[2] = { 0, };
	uint32_t R[3] = { 0, };
	uint32_t sum = 0;
	uint32_t carry = 0;

	BIGNUM tmpA = { 0, };
	BIGNUM tmpB = { 0, };
	
	for (int i = 0; i < a->top; i++)
	{
		tmpA.d[i] = a->d[i];
	}
	for (int i = 0; i < b->top; i++)
	{
		tmpB.d[i] = b->d[i];
	}
	tmpA.top = a->top;
	tmpB.top = b->top;

	for (int k = 0; k < tmpA.top + tmpB.top - 1; k++)
	{
		for (int i = 0; i < tmpA.top; i++)
		{
			int j = k - i;
			if (j < 0)
			{
				break;
			}
			divisionMul(UV, tmpA.d[i], tmpB.d[j]);
			carry = 0;
			sum = R[0] + UV[0];
			if (sum < R[0])
			{
				carry = 1;
			}
			R[0] = sum;

			sum = R[1] + carry;
			carry = 0;
			if (sum < R[1])
			{
				carry = 1;
			}
			sum += UV[1];
			if (sum < UV[1])
			{
				carry += 1;
			}
			R[1] = sum;

			R[2] += carry;
		}
		r->d[k] = R[0];
		R[0] = R[1];
		R[1] = R[2];
		R[2] = 0;
	}
	r->top = tmpA.top + tmpB.top;
	r->d[r->top - 1] = R[0];
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

void Squaring(BIGNUM* r, const BIGNUM* a)
{
	uint32_t UV[2] = { 0, };
	uint32_t R[3] = { 0, };
	uint32_t sum = 0;
	uint32_t carry = 0;
	uint32_t cbit1 = 0, cbit2 = 0;

	BIGNUM tmpA = { 0, };

	for (int i = 0; i < a->top; i++)
	{
		tmpA.d[i] = a->d[i];
	}
	tmpA.top = a->top;

	for (int k = 0; k < tmpA.top * 2 - 1; k++)
	{
		for (int i = 0; i <= k / 2; i++)
		{
			int j = k - i;
			divisionMul(UV, tmpA.d[i], tmpA.d[j]);
			if (i != j)
			{
				// UV 2배 해주기
				cbit1 = UV[0] >> 31;
				UV[0] <<= 1;
				cbit2 = UV[1] >> 31;
				UV[1] <<= 1;
				UV[1] += cbit1;
				R[2] += cbit2;
			}
			carry = 0;
			sum = R[0] + UV[0];
			if (sum < R[0])
			{
				carry = 1;
			}
			R[0] = sum;

			sum = R[1] + carry;
			carry = 0;
			if (sum < R[1])
			{
				carry = 1;
			}
			sum += UV[1];
			if (sum < UV[1])
			{
				carry += 1;
			}
			R[1] = sum;

			R[2] += carry;
		}
		r->d[k] = R[0];
		R[0] = R[1];
		R[1] = R[2];
		R[2] = 0;
	}
	r->top = tmpA.top * 2;
	r->d[r->top - 1] = R[0];
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
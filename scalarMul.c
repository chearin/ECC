#include "scalarMul.h"

void LtRMul(BIGNUMPOINT* r, const BIGNUM* k, const BIGNUMPOINT* a, const BIGNUM* P)
{
	BIGNUM px = { 0 };
	BIGNUM py = { 0 };
	BIGNUM qx = { 0 };
	BIGNUM qy = { 0 };
	BIGNUM zero = { {0}, 0, 0 };
	BIGNUMPOINT p = { 0, };
	BIGNUMPOINTJACO q = { 0, };
	
	//Affine Point인 a를 복사
	for (int i = 0; i < a->x->top; i++)
	{
		px.d[i] = a->x->d[i];
	}
	px.top = a->x->top;
	for (int i = 0; i < a->y->top; i++)
	{
		py.d[i] = a->y->d[i];
	}
	py.top = a->y->top;

	initPoint(&p, &px, &py);
	initPointJA(&q, &qx, &qy, &zero);

	for (int i = k->top - 1; i >= 0; i--)
	{
		for (int j = 31; j >= 0; j--)
		{
			ECDBLJ(&q, &q, P);
			if ((k->d[i] >> j) & 0x00000001)
			{
				ECADDJ(&q, &q, &p, P);
			}
		}
	}
	J2A(r, &q, P);
}

void RtLMul(BIGNUMPOINT* r, const BIGNUM* k, const BIGNUMPOINT* a, const BIGNUM* P)
{
	BIGNUM px = { 0 };
	BIGNUM py = { 0 };
	BIGNUM qax = { 0 };
	BIGNUM qay = { 0 };
	BIGNUM qjx = { 0 };
	BIGNUM qjy = { 0 };
	BIGNUM qjz = { 0 };
	BIGNUM one = { {1, }, 1, 0 };
	BIGNUMPOINTJACO pj = { 0, };
	BIGNUMPOINT qa = { 0, };
	BIGNUMPOINTJACO qj = { 0, };

	//Affine Point인 a를 복사
	for (int i = 0; i < a->x->top; i++)
	{
		px.d[i] = a->x->d[i];
	}
	px.top = a->x->top;
	for (int i = 0; i < a->y->top; i++)
	{
		py.d[i] = a->y->d[i];
	}
	py.top = a->y->top;

	initPointJA(&pj, &px, &py, &one);
	initPoint(&qa, &qax, &qay);
	initPointJA(&qj, &qjx, &qjy, &qjz);

	for (int i = 0; i < k->top; i++)
	{
		for (int j = 0; j < 32; j++)
		{
			if ((k->d[i] >> j) & 0x00000001)
			{
				ECADDJ(&qj, &pj, &qa, P);
				J2A(&qa, &qj, P);
			}
			ECDBLJ(&pj, &pj, P);
		}
	}

	for (int i = 0; i < (qa.x->top); i++)
	{
		r->x->d[i] = qa.x->d[i];
	}
	r->x->top = qa.x->top;
	for (int i = 0; i < (qa.y->top); i++)
	{
		r->y->d[i] = qa.y->d[i];
	}
	r->y->top = qa.y->top;
}
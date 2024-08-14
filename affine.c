#include "affine.h"

void ECADD(BIGNUMPOINT* r, const BIGNUMPOINT* p1, const BIGNUMPOINT* p2, const BIGNUM* P)
{
	BIGNUM temp1 = { 0, };
	BIGNUM temp2 = { 0, };
	BIGNUM temp3 = { 0, };

	//x3 = ((y2 - y1)/(x2 - x1))^2 - x1 - x2
	PF_substraction(&temp1, P, p2->y, p1->y);
	PF_substraction(&temp2, P, p2->x, p1->x);
	FLT256(&temp2, P, &temp2);
	OperandScanning(&temp1, &temp1, &temp2);
	fastReduction2(&temp1, &temp1, P);
	BignumberCopy(&temp3, &temp1);
	Squaring(&temp1, &temp1);
	fastReduction2(&temp1, &temp1, P);
	PF_substraction(&temp1, P, &temp1, p1->x);
	PF_substraction(&temp1, P, &temp1, p2->x);

	for (int i = 0; i < temp1.top; i++)
	{
		r->x->d[i] = temp1.d[i];
	}
	r->x->top = temp1.top;

	//y3 = (y2 - y1)/(x2 - x1)*(x1 - x3) - y1
	PF_substraction(&temp1, P, p1->x, r->x);
	OperandScanning(&temp1, &temp3, &temp1);
	fastReduction2(&temp1, &temp1, P);
	PF_substraction(&temp1, P, &temp1, p1->y);

	for (int i = 0; i < temp1.top; i++)
	{
		r->y->d[i] = temp1.d[i];
	}
	r->y->top = temp1.top;
}

void ECDBL(BIGNUMPOINT* r, const ECURVE* e, const BIGNUMPOINT* p1, const BIGNUM* P)
{
	BIGNUM temp1 = { 0, };
	BIGNUM temp2 = { 0, };
	BIGNUM temp3 = { 0, };

	//x3 = ((3*x1^2 + a)/(2*y1))^2 - 2*x1
	Squaring(&temp1, p1->x);
	fastReduction2(&temp1, &temp1, P);
	BignumberCopy(&temp3, &temp1);
	PF_addition(&temp1, P, &temp1, &temp1);
	PF_addition(&temp1, P, &temp1, &temp3);
	PF_substraction(&temp1, P, &temp1, e->a);
	PF_addition(&temp2, P, p1->y, p1->y);
	FLT256(&temp2, P, &temp2);
	OperandScanning(&temp1, &temp1, &temp2);
	fastReduction2(&temp1, &temp1, P);
	BignumberCopy(&temp3, &temp1);
	Squaring(&temp1, &temp1);
	fastReduction2(&temp1, &temp1, P);
	PF_addition(&temp2, P, p1->x, p1->x);
	PF_substraction(&temp1, P, &temp1, &temp2);

	for (int i = 0; i < temp1.top; i++)
	{
		r->x->d[i] = temp1.d[i];
	}
	r->x->top = temp1.top;

	//y3 = (3*x1^2 + a)/(2*y1)*(x1 - x3) - y1
	PF_substraction(&temp1, P, p1->x, r->x);
	OperandScanning(&temp1, &temp3, &temp1);
	fastReduction2(&temp1, &temp1, P);
	PF_substraction(&temp1, P, &temp1, p1->y);

	for (int i = 0; i < temp1.top; i++)
	{
		r->y->d[i] = temp1.d[i];
	}
	r->y->top = temp1.top;
}
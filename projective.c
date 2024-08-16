#include "projective.h"

void ECADDJ(BIGNUMPOINTJACO* r, const BIGNUMPOINTJACO* p1, const BIGNUMPOINT* p2, const BIGNUM* P)
{
	BIGNUM t1 = { 0, };
	BIGNUM t2 = { 0, };
	BIGNUM t3 = { 0, };
	BIGNUM t4 = { 0, };
	BIGNUM x3 = { 0, };
	BIGNUM y3 = { 0, };
	BIGNUM z3 = { 0, };

	BIGNUM zero = { {0, }, };
	BIGNUM one = { {1, },1, 0 };
	BIGNUM two = { {2, },1, 0 };
	BIGNUM three = { {3, },1, 0 };

	//x3 = (y2*z1^3 - y1)^2 - (x2*z1^2 - x1)^2*(x1 + x2*z1^2)
	//y3 = (y2*z1^3 - y1)*(x1*(x2*z1^2 - x1)^2 - x3) - y1*(x2*z1^2 - x1)^3
	//z3 = z1*(x2*z1^2 - x1)
	Squaring(&t1, p1->z);
	fastReduction2(&t1, &t1, P);
	OperandScanning(&t2, &t1, p1->z);
	fastReduction2(&t2, &t2, P);
	OperandScanning(&t1, &t1, p2->x);
	fastReduction2(&t1, &t1, P);
	OperandScanning(&t2, &t2, p2->y);
	fastReduction2(&t2, &t2, P);
	PF_substraction(&t1, P, &t1, p1->x);
	PF_substraction(&t2, P, &t2, p1->y);
	// Algorithm line9: if문
	if (compare(&t1, &zero) == 0) //z3 = 0
	{
		BIGNUMPOINTJACO R = { 0, };
		if (compare(&t2, &zero) == 0) //y3 = 0
		{
			//2(x2:y2:1) return
			initPointJA(&R, p2->x, p2->y, &one);
			ECDBLJ(&R, &R, P);
		}
		else
		{
			//infinity return
			initPointJA(&R, p2->x, p2->y, &zero);
		}

		for (int i = 0; i < R.x->top; i++)
		{
			r->x->d[i] = R.x->d[i];
		}
		r->x->top = R.x->top;

		for (int i = 0; i < R.y->top; i++)
		{
			r->y->d[i] = R.y->d[i];
		}
		r->y->top = R.y->top;

		for (int i = 0; i < R.z->top; i++)
		{
			r->z->d[i] = R.z->d[i];
		}
		r->z->top = R.z->top;
		return;
	}
	//
	OperandScanning(&z3, p1->z, &t1);
	fastReduction2(&z3, &z3, P);
	Squaring(&t3, &t1);
	fastReduction2(&t3, &t3, P);
	OperandScanning(&t4, &t3, &t1);
	fastReduction2(&t4, &t4, P);
	OperandScanning(&t3, &t3, p1->x);
	fastReduction2(&t3, &t3, P);
	OperandScanning(&t1, &t3, &two);
	fastReduction2(&t1, &t1, P);
	Squaring(&x3, &t2);
	fastReduction2(&x3, &x3, P);
	PF_substraction(&x3, P, &x3, &t1);
	PF_substraction(&x3, P, &x3, &t4);
	PF_substraction(&t3, P, &t3, &x3);
	OperandScanning(&t3, &t3, &t2);
	fastReduction2(&t3, &t3, P);
	OperandScanning(&t4, &t4, p1->y);
	fastReduction2(&t4, &t4, P);
	PF_substraction(&y3, P, &t3, &t4);

	for (int i = 0; i < x3.top; i++)
	{
		r->x->d[i] = x3.d[i];
	}
	r->x->top = x3.top;

	for (int i = 0; i < y3.top; i++)
	{
		r->y->d[i] = y3.d[i];
	}
	r->y->top = y3.top;

	for (int i = 0; i < z3.top; i++)
	{
		r->z->d[i] = z3.d[i];
	}
	r->z->top = z3.top;
}

// curve의 a값은 -3이라고 함
void ECDBLJ(BIGNUMPOINTJACO* r, const BIGNUMPOINTJACO* p1, const BIGNUM* P)
{
	BIGNUM t1 = { 0, };
	BIGNUM t2 = { 0, };
	BIGNUM t3 = { 0, };
	BIGNUM x3 = { 0, };
	BIGNUM y3 = { 0, };
	BIGNUM z3 = { 0, };
	BIGNUM twoinv = { 0, };

	BIGNUM zero = { {0, }, };
	BIGNUM two = { {2, },1,0 };
	BIGNUM three = { {3, },1,0 };

	//infinity return
	if (compare(p1->z, &zero) == 0)
	{
		BIGNUMPOINTJACO R = { 0, };
		initPointJA(&R, p1->x, p1->y, &zero);

		for (int i = 0; i < R.x->top; i++)
		{
			r->x->d[i] = R.x->d[i];
		}
		r->x->top = R.x->top;

		for (int i = 0; i < R.y->top; i++)
		{
			r->y->d[i] = R.y->d[i];
		}
		r->y->top = R.y->top;

		for (int i = 0; i < R.z->top; i++)
		{
			r->z->d[i] = R.z->d[i];
		}
		r->z->top = R.z->top;
		return;
	}

	//x3 = (3*x1^2 + a*z1^4)^2 - 8*x1*y1^2
	//y3 = (3*x1^2 + a*z1^4)*(4*x1*y1^2 - x3) - 8*y1^4
	//z3 = 2*y1*z1
	Squaring(&t1, p1->z);
	fastReduction2(&t1, &t1, P);
	PF_substraction(&t2, P, p1->x, &t1);
	PF_addition(&t1, P, p1->x, &t1);
	OperandScanning(&t2, &t2, &t1);
	fastReduction2(&t2, &t2, P);
	OperandScanning(&t2, &t2, &three);
	fastReduction2(&t2, &t2, P);
	OperandScanning(&y3, p1->y, &two);
	fastReduction2(&y3, &y3, P);
	OperandScanning(&z3, &y3, p1->z);
	fastReduction2(&z3, &z3, P);
	Squaring(&y3, &y3);
	fastReduction2(&y3, &y3, P);
	OperandScanning(&t3, &y3, p1->x);
	fastReduction2(&t3, &t3, P);
	Squaring(&y3, &y3);
	fastReduction2(&y3, &y3, P);
	//BignumberRShift(&y3, &y3, 1);
	FLT256(&twoinv, P, &two);
	OperandScanning(&y3, &y3, &twoinv);
	fastReduction2(&y3, &y3, P);
	//
	Squaring(&x3, &t2);
	fastReduction2(&x3, &x3, P);
	OperandScanning(&t1, &t3, &two);
	fastReduction2(&t1, &t1, P);
	PF_substraction(&x3, P, &x3, &t1);
	PF_substraction(&t1, P, &t3, &x3);
	OperandScanning(&t1, &t1, &t2);
	fastReduction2(&t1, &t1, P);
	PF_substraction(&y3, P, &t1, &y3);

	for (int i = 0; i < x3.top; i++)
	{
		r->x->d[i] = x3.d[i];
	}
	r->x->top = x3.top;

	for (int i = 0; i < y3.top; i++)
	{
		r->y->d[i] = y3.d[i];
	}
	r->y->top = y3.top;

	for (int i = 0; i < z3.top; i++)
	{
		r->z->d[i] = z3.d[i];
	}
	r->z->top = z3.top;
}

// Jacobian->Affine
void J2A(BIGNUMPOINT* r, const BIGNUMPOINTJACO* a, const BIGNUM* P)
{
	BIGNUM t1 = { 0, };
	BIGNUM t2 = { 0, };

	BIGNUM zero = { {0, }, };

	//a->z가 0이면 infinity
	if (compare(a->z, &zero) == 0)
	{
		printf("Infinity error: conversion to Affine failed..\n");
		return;
	}
	Squaring(&t1, a->z);
	fastReduction2(&t1, &t1, P);
	OperandScanning(&t1, &t1, a->z);
	fastReduction2(&t1, &t1, P);
	FLT256(&t1, P, &t1);
	OperandScanning(r->y, a->y, &t1);
	fastReduction2(r->y, r->y, P);
	OperandScanning(&t1, &t1, a->z);
	fastReduction2(&t1, &t1, P);
	OperandScanning(r->x, a->x, &t1);
	fastReduction2(r->x, r->x, P);
}
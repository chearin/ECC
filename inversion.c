#include "inversion.h"

void BignumberCopy(BIGNUM* d, const BIGNUM* r)
{
	for (int i = 0; i < r->top; i++)
	{
		d->d[i] = r->d[i];
	}
	d->top = r->top;
}

void BinaryLongDivision(uint32_t* q, uint32_t* r, const uint32_t a, const uint32_t b)
{
	//// R < b, b는 32비트 이내, R << 1에서 초과 비트 없음
	//uint32_t R = 0;

	//for (int i = 7; i >= 0; i++)
	//{
	//	R = R << 1 + ((a >> i) & 0b00000001);
	//	if (R > b)
	//	{

	//	}
	//}
}

void BignumberDivision(BIGNUM* q, BIGNUM* r, const BIGNUM* a, const BIGNUM* b)
{
	//BIGNUM R = { 0, };
	////몫과 나머지 초기화
	//for (int i = 0; i < q->top; i++)
	//{
	//	q->d[i] = 0;
	//}
	//q->top = 0;
	//for (int i = 0; i < r->top; i++)
	//{
	//	r->d[i] = 0;
	//}
	//r->top = 0;

	

}

void ExtendedEuclidean(BIGNUM* inv, const BIGNUM* P, const BIGNUM* a)
{
	//BIGNUM u = { 0, };
	//BIGNUM v = { 0, };
	//BIGNUM x = { 0, };
	//BIGNUM x1 = { 0, };
	//BIGNUM x2 = { 0, };
	//BIGNUM q = { 0, };
	//BIGNUM r = { 0, };

	//BIGNUM one = { 0, };
	//one.d[0] = 1;
	//one.top = 1;

	//x1.d[0] = 1;
	//x1.top = 1;

	//x2.d[0] = 0;
	//x1.top = 1;

	//while (compare(&one, &u) != 0)
	//{
	//	BignumberDivision(&q, &r, &v, &u);
	//	karaAdd(&x1, &q, &x1);
	//	BignumberSub(&x, &x2, &x1);

	//	BignumberCopy(&v, &u);
	//	BignumberCopy(&u, &r);
	//	BignumberCopy(&x2, &x1);
	//	BignumberCopy(&x1, &x);
	//}
	//fastReduction(&inv, &x1, P);
}

void FLT(BIGNUM* inv, const BIGNUM* P, const BIGNUM* a)
{
	uint8_t bit = 0;
	BIGNUM exp = { 0, };
	BIGNUM two = { 0, };

	two.d[0] = 2;
	two.top = 1;
	
	//exp에 P를 copy
	for (int i = 0; i < P->top; i++)
	{
		exp.d[i] = P->d[i];
	}
	exp.top = P->top;

	//exp = P - 2
	BignumberSub(&exp, &exp, &two);

	inv->d[0] = 1;
	inv->top = 1;

	//a^(-1) = a^(p-2)
	for (int i = exp.top - 1; i >= 0; i--)
	{
		for (int j = 31; j >= 0; j--)
		{
			Squaring(inv, inv);
			fastReduction(inv, inv, P);
			if ((exp.d[i] >> j) & 0x00000001)
			{
				OperandScanning(inv, inv, a);
				fastReduction(inv, inv, P);
			}
		}
	}
}

void FLT256(BIGNUM* inv, const BIGNUM* P, const BIGNUM* a)
{
	BIGNUM tmp = { 0, };
	BIGNUM z3 = { 0, };
	BIGNUM z15 = { 0, };
	BIGNUM t0 = { 0, };
	BIGNUM t1 = { 0, };
	BIGNUM t2 = { 0, };
	BIGNUM t3 = { 0, };
	BIGNUM t4 = { 0, };
	BIGNUM t5 = { 0, };
	
	//z3
	Squaring(&tmp, a);
	fastReduction(&tmp, &tmp, P);
	OperandScanning(&z3, &tmp, a);
	fastReduction(&z3, &z3, P);

	//z15
	Squaring(&tmp, &z3);
	fastReduction(&tmp, &tmp, P);
	for (int i = 1; i < 2; i++)
	{
		Squaring(&tmp, &tmp);
		fastReduction(&tmp, &tmp, P);
	}
	OperandScanning(&z15, &tmp, &z3);
	fastReduction(&z15, &z15, P);

	//t0
	Squaring(&tmp, &z15);
	fastReduction(&tmp, &tmp, P);
	for (int i = 1; i < 2; i++)
	{
		Squaring(&tmp, &tmp);
		fastReduction(&tmp, &tmp, P);
	}
	OperandScanning(&t0, &tmp, &z3);
	fastReduction(&t0, &t0, P);

	//t1
	Squaring(&tmp, &t0);
	fastReduction(&tmp, &tmp, P);
	for (int i = 1; i < 6; i++)
	{
		Squaring(&tmp, &tmp);
		fastReduction(&tmp, &tmp, P);
	}
	OperandScanning(&t1, &tmp, &t0);
	fastReduction(&t1, &t1, P);

	//t2
	Squaring(&tmp, &t1);
	fastReduction(&tmp, &tmp, P);
	for (int i = 1; i < 12; i++)
	{
		Squaring(&tmp, &tmp);
		fastReduction(&tmp, &tmp, P);
	}
	OperandScanning(&tmp, &tmp, &t1);
	fastReduction(&tmp, &tmp, P);
	for (int i = 0; i < 6; i++)
	{
		Squaring(&tmp, &tmp);
		fastReduction(&tmp, &tmp, P);
	}
	OperandScanning(&t2, &tmp, &t0);
	fastReduction(&t2, &t2, P);

	//t3
	Squaring(&tmp, &t2);
	fastReduction(&tmp, &tmp, P);
	for (int i = 1; i < 2; i++)
	{
		Squaring(&tmp, &tmp);
		fastReduction(&tmp, &tmp, P);
	}
	OperandScanning(&t3, &tmp, &z3);
	fastReduction(&t3, &t3, P);

	//t4
	Squaring(&tmp, &t3);
	fastReduction(&tmp, &tmp, P);
	for (int i = 1; i < 32; i++)
	{
		Squaring(&tmp, &tmp);
		fastReduction(&tmp, &tmp, P);
	}
	OperandScanning(&tmp, &tmp, a);
	fastReduction(&tmp, &tmp, P);
	for (int i = 0; i < 95; i++)
	{
		Squaring(&tmp, &tmp);
		fastReduction(&tmp, &tmp, P);
	}
	Squaring(&t4, &tmp);
	fastReduction(&t4, &t4, P);

	//t5
	Squaring(&tmp, &t4);
	fastReduction(&tmp, &tmp, P);
	for (int i = 1; i < 32; i++)
	{
		Squaring(&tmp, &tmp);
		fastReduction(&tmp, &tmp, P);
	}
	OperandScanning(&tmp, &tmp, &t3);
	fastReduction(&tmp, &tmp, P);
	for (int i = 0; i < 32; i++)
	{
		Squaring(&tmp, &tmp);
		fastReduction(&tmp, &tmp, P);
	}
	OperandScanning(&t5, &tmp, &t3);
	fastReduction(&t5, &t5, P);

	//t
	Squaring(&tmp, &t5);
	fastReduction(&tmp, &tmp, P);
	for (int i = 1; i < 30; i++)
	{
		Squaring(&tmp, &tmp);
		fastReduction(&tmp, &tmp, P);
	}
	OperandScanning(&tmp, &tmp, &t2);
	fastReduction(&tmp, &tmp, P);
	for (int i = 0; i < 2; i++)
	{
		Squaring(&tmp, &tmp);
		fastReduction(&tmp, &tmp, P);
	}
	OperandScanning(inv, &tmp, a);
	fastReduction(inv, inv, P);
}
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

void Naf(NAF* r, const BIGNUM* scalar)
{
	BIGNUM zero = { 0, };
	BIGNUM one = { {1,},1, 0 };
	BIGNUM k = { 0, };
	int8_t tmp = 0;

	for (int i = 0; i < scalar->top; i++)
	{
		k.d[i] = scalar->d[i];
	}
	k.top = scalar->top;

	int i = 0;
	while (compare(&k, &zero) == 1)
	{
		if (k.d[0] & 0x00000001)
		{
			//tmp = 1 or tmp = -1
			tmp = 2 - (k.d[0] & 0x00000003);
			r->d[i] = tmp;
			if (tmp == 1)
			{
				BignumberSub(&k, &k, &one);
			}
			else
			{
				BignumberAdd(&k, &k, &one);
			}
		}
		else
		{
			r->d[i] = 0;
		}
		BignumberRShift(&k, &k, 1);
		i++;
	}
	r->top = i;
}

// k->d[point] 부터 window 길이를 확인
void windowFindOdd(uint8_t* u, uint8_t* t, const NAF* k, const uint8_t w, const uint8_t point)
{
	*t = w;
	int j = point;
	for (int i = w - 1; i >= 0; i--)
	{
		if (k->d[j - i] != 0)
		{
			break;
		}
		j--;
		*t -= 1;
	}
	//u는 window 안에서의 최대 홀수, LtR으로 계산
	*u = 0;
	for (int i = 0; i < t; i++)
	{
		*u *= 2;
		*u += k->d[point - i];
	}
}

void SlidingWindow(BIGNUMPOINT* r, const BIGNUM* scalar, const BIGNUMPOINT* a, const uint8_t w, const BIGNUM* P)
{
	BIGNUM tmpx = { 0, };
	BIGNUM tmpy = { 0, };
	BIGNUM tmpz = { 0, };
	BIGNUM px2 = { 0, };
	BIGNUM py2 = { 0, };
	BIGNUM qx = { 0, };
	BIGNUM qy = { 0, };
	BIGNUM qz = { 0, };
	BIGNUM invy = { 0, };
	BIGNUM zero = { {0}, 0, 0 };
	BIGNUMPOINTJACO tmp = { 0, };
	BIGNUMPOINT p2 = { 0, }; //2P
	BIGNUMPOINTJACO q = { 0, };
	BIGNUMPOINTJACO inv = { 0, };
	BIGNUMPOINTJACO2 PP[256] = { 0, };
	
	NAF k = { 0, };
	
	uint8_t max = 1;
	uint8_t tm = 1;
	uint8_t t = 0;
	uint8_t u = 0;
	int i = 0;

	initPointJA(&tmp, &tmpx, &tmpy, &tmpz);
	initPoint(&p2, &px2, &py2);
	initPointJA(&q, &qx, &qy, &qz);
	Naf(&k, scalar);

	for (int i = 0; i < a->x->top; i++)
	{
		PP[0].x.d[i] = a->x->d[i];
	}
	PP[0].x.top = a->x->top;
	for (int i = 0; i < a->y->top; i++)
	{
		PP[0].y.d[i] = a->y->d[i];
	}
	PP[0].y.top = a->y->top;
	PP[0].z.d[0] = 1;
	PP[0].z.top = 1;

	//사전계산
	// 2*(2^w - (-1)^w)/3 - 1
	for (int i = 0; i < w; i++)
	{
		max *= 2;
		tm *= -1;
	}
	max -= tm;
	max /= 3;
	//PP[0] = P, PP[1] = 3P, PP[2] = 5P ...
	ECDBLJ(&tmp, &PP[0], P);
	J2A(&p2, &tmp, P);
	for (int i = 1; i <= max; i++)
	{
		ECADDJ(&PP[i], &PP[i - 1], &p2, P);
	}
	
	i = k.top - 1;
	while (i >= 0)
	{
		if (k.d[i] == 0)
		{
			t = 1;
			u = 0;
		}
		else
		{
			windowFindOdd(&u, &t, &k, w, i);
		}
		for (int j = 0; j < t; j++)
		{
			ECDBLJ(&q, &q, P);
		}
		if (u > 0)
		{
			J2A(&p2, &PP[(u - 1) / 2], P);
			ECADDJ(&q, &q, &p2, P);
		}
		else if(u < 0)
		{
			PF_substraction(&invy, P, &zero, &PP[(-u - 1) / 2].y);
			initPointJA(&inv, &PP[(-u - 1) / 2].x, &invy, &PP[(-u - 1) / 2].z);
			J2A(&p2, &inv, P);			
			ECADDJ(&q, &q, &p2, P);
		}
		i -= t;
	}
}
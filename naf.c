#include "naf.h"

void initNaf(NAF* N, const int32_t* x, const size_t len)
{
	for (int i = 0; i < len; i++)
	{
		N->d[i] = x[i];
	}
	for (int i = len; i < 256; i++)
	{
		N->d[i] = 0;
	}
	N->top = len;
}
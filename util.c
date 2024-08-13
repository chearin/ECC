#define _CRT_SECURE_NO_WARNINGS
#include "util.h"

FILE* fileOpen(char* fname, char* mode)
{
	FILE* fp = fopen(fname, mode);

	if (fp == NULL)
	{
		printf("file open error(%s)..\n", fname);
	}

	return fp;
}

//hexlen은 uint32_t형 배열 hex의 길이
void str2hex(const unsigned char* str, uint32_t* hex, size_t hexlen)
{
	unsigned char buffer[3] = { 0 };

	for (int i = 0; i < hexlen; i++)
	{
		buffer[0] = str[8 * (hexlen - 1 - i)];
		buffer[1] = str[8 * (hexlen - 1 - i) + 1];
		buffer[2] = '\0';
		hex[i] = strtol(buffer, NULL, 16);
		hex[i] <<= 8;

		buffer[0] = str[8 * (hexlen - 1 - i) + 2];
		buffer[1] = str[8 * (hexlen - 1 - i) + 3];
		buffer[2] = '\0';
		hex[i] ^= strtol(buffer, NULL, 16);
		hex[i] <<= 8;

		buffer[0] = str[8 * (hexlen - 1 - i) + 4];
		buffer[1] = str[8 * (hexlen - 1 - i) + 5];
		buffer[2] = '\0';
		hex[i] ^= strtol(buffer, NULL, 16);
		hex[i] <<= 8;

		buffer[0] = str[8 * (hexlen - 1 - i) + 6];
		buffer[1] = str[8 * (hexlen - 1 - i) + 7];
		buffer[2] = '\0';
		hex[i] ^= strtol(buffer, NULL, 16);
	}
}
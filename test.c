#define _CRT_SECURE_NO_WARNINGS
#include "test.h"

void ECADDDBL()
{
	FILE* fp0 = NULL;
	FILE* fp1 = NULL;
	FILE* fp2 = NULL;
	FILE* fp3 = NULL;

	unsigned char input_str[1000] = { 0 };
	uint32_t P256[8] = { 0 };
	uint32_t opA[8] = { 0 };
	uint32_t opB[8] = { 0 };
	uint32_t opC[8] = { 0 };

	BIGNUM P = { 0, };
	BIGNUM ax = { 0, };
	BIGNUM ay = { 0, };
	BIGNUM az = { 0, };
	BIGNUM bx = { 0, };
	BIGNUM by = { 0, };
	BIGNUM bz = { 0, };
	BIGNUM rx = { 0, };
	BIGNUM ry = { 0, };
	BIGNUM rz = { 0, };
	BIGNUM cur_a = { {2,},1,0 };
	BIGNUM cur_b = { {3,},1,0 };

	BIGNUMPOINT A = { 0, };
	BIGNUMPOINT B = { 0, };
	BIGNUMPOINT RA = { 0, };

	BIGNUMPOINTJACO AJ = { 0, };
	BIGNUMPOINTJACO RJ = { 0, };

	ECURVE CUR1 = { 0, };
	size_t len = 0;

	unsigned long long start = 0, end = 0;
	unsigned long long ECADDcc = 0, ECDBLcc = 0;
	unsigned long long JAECADDcc = 0, JAECDBLcc = 0;
	int count = 0;

	fp0 = fileOpen("P256값.txt", "r");
	fp1 = fileOpen("중간결과값.txt", "r");
	fp2 = fileOpen("ECDBL.txt", "w");
	fp3 = fileOpen("ECADD.txt", "w");

	//p256값 저장
	fscanf(fp0, "%s", input_str);
	len = strlen(input_str) / (sizeof(int) * 2);
	str2hex(input_str, P256, len);
	initBignum(P256, len, &P);

	//테스트 벡터들 읽기
	fscanf(fp1, "1. 1 x G = (");
	for (int i = 0; i < 8; i++)
	{
		fscanf(fp1, "%08x", &opA[i]);
	}
	fscanf(fp1, ", ");
	for (int i = 0; i < 8; i++)
	{
		fscanf(fp1, "%08x", &opB[i]);
	}

	//Affine doubling
	initBignum(&opA, 8, &ax);
	initBignum(&opB, 8, &ay);
	initPoint(&A, &ax, &ay);
	initPoint(&RA, &rx, &ry);
	initEcurve(&CUR1, &cur_a, &cur_b);

	start = cpucycles();
	ECDBL(&RA, &CUR1, &A, &P);
	end = cpucycles();
	ECDBLcc += (end - start);

	//결과값 파일에 쓰기
	for (int i = RA.x->top - 1; i >= 0; i--)
	{
		fprintf(fp2, "%08X", RA.x->d[i]);
	}
	fprintf(fp2, "\n");
	for (int i = RA.y->top - 1; i >= 0; i--)
	{
		fprintf(fp2, "%08X", RA.y->d[i]);
	}
	fprintf(fp2, "\n\n");

	//Affine addition
	initPoint(&A, &ax, &ay);
	initPoint(&B, &bx, &by);
	
	start = cpucycles();
	ECADD(&RA, &A, &RA, &P);
	end = cpucycles();
	ECADDcc += (end - start);

	//결과값 파일에 쓰기
	for (int i = RA.x->top - 1; i >= 0; i--)
	{
		fprintf(fp3, "%08X", RA.x->d[i]);
	}
	fprintf(fp3, "\n");
	for (int i = RA.y->top - 1; i >= 0; i--)
	{
		fprintf(fp3, "%08X", RA.y->d[i]);
	}
	fprintf(fp3, "\n\n");	

	////Jacobian addition, doubling
	//initPointJA(&AJ, &ax, &ay, &az);
	//initPoint(&B, &bx, &by);
	//initPointJA(&RJ, &rx, &ry, &rz);
	//initPoint(&RA, &rx, &ry);

	//start = cpucycles();
	//ECADDJ(&RJ, &AJ, &B, &P);
	//end = cpucycles();
	//JAECADDcc += (end - start);
	//J2A(&RA, &RJ, &P);

	//initPointJA(&AJ, &ax, &ay, &az);
	//initPointJA(&RJ, &rx, &ry, &rz);
	//initPoint(&RA, &rx, &ry);

	//start = cpucycles();
	//ECDBLJ(&RJ, &AJ, &P);
	//end = cpucycles();
	//JAECDBLcc += (end - start);
	//J2A(&RA, &RJ, &P);	

	printf("ECADDcc = %d\n", ECADDcc / count);
	printf("ECDBLcc = %d\n", ECDBLcc / count);
	//printf("JAECADDcc = %d\n", JAECADDcc / count);
	//printf("JAECDBLcc = %d\n", JAECDBLcc / count);

	fclose(fp0);
	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
}
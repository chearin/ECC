#define _CRT_SECURE_NO_WARNINGS
#include "test.h"

void ECADDDBL()
{
	FILE* fp0 = NULL;
	FILE* fp1 = NULL;
	FILE* fp2 = NULL;
	FILE* fp3 = NULL;
	FILE* fp4 = NULL;
	FILE* fp5 = NULL;

	unsigned char input_str[1000] = { 0 };
	uint32_t P256[8] = { 0 };
	uint32_t opA[8] = { 0 };
	uint32_t opB[8] = { 0 };
	uint32_t opC[8] = { 0 };

	BIGNUM P = { 0, };
	BIGNUM ax = { 0, };
	BIGNUM ay = { 0, };
	BIGNUM az = { {1,},1,0 };
	BIGNUM bx = { 0, };
	BIGNUM by = { 0, };
	BIGNUM bz = { {1,},1,0 };
	BIGNUM rx = { 0, };
	BIGNUM ry = { 0, };
	BIGNUM rjx = { 0, };
	BIGNUM rjy = { 0, };
	BIGNUM rjz = { {1,},1,0 };
	BIGNUM rjz2 = { {1,},1,0 };
	BIGNUM cur_a = { {3,},1,0 };
	BIGNUM cur_b = { {0,},0,0 };

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

	fp0 = fileOpen("P256값.txt", "r");
	fp1 = fileOpen("중간결과값.txt", "r");
	fp2 = fileOpen("ECDBL.txt", "w");
	fp3 = fileOpen("ECADD.txt", "w");
	fp4 = fileOpen("ECDBLJA.txt", "w");
	fp5 = fileOpen("ECADDJA.txt", "w");

	//p256값 저장
	fscanf(fp0, "%s", input_str);
	len = strlen(input_str) / (sizeof(int) * 2);
	str2hex(input_str, P256, len);
	initBignum(P256, len, &P);

	//테스트 벡터들 읽기
	fscanf(fp1, "1. 1 x G = (");
	for (int i = 7; i >= 0; i--)
	{
		fscanf(fp1, "%08x", &opA[i]);
	}
	fscanf(fp1, ", ");
	for (int i = 7; i >= 0; i--)
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
	printf("DBL: ");
	fprintf(fp2, "DBL: ");
	for (int i = RA.x->top - 1; i >= 0; i--)
	{
		printf("%08x", RA.x->d[i]);
		fprintf(fp2, "%08x", RA.x->d[i]);
	}
	printf(", ");
	fprintf(fp2, ", ");
	for (int i = RA.y->top - 1; i >= 0; i--)
	{
		printf("%08x", RA.y->d[i]);
		fprintf(fp2, "%08x", RA.y->d[i]);
	}
	fprintf(fp2, "\n\n");
	printf("\n\n");

	//Affine addition	
	start = cpucycles();
	ECADD(&RA, &A, &RA, &P);
	end = cpucycles();
	ECADDcc += (end - start);

	//결과값 파일에 쓰기
	printf("ADD: ");
	fprintf(fp3, "ADD: ");
	for (int i = RA.x->top - 1; i >= 0; i--)
	{
		printf("%08x", RA.x->d[i]);
		fprintf(fp3, "%08x", RA.x->d[i]);
	}
	printf(", ");
	fprintf(fp3, ", ");
	for (int i = RA.y->top - 1; i >= 0; i--)
	{
		printf("%08x", RA.y->d[i]);
		fprintf(fp3, "%08x", RA.y->d[i]);
	}
	printf("\n\n");
	fprintf(fp3, "\n\n");

	//Jacobian doubling
	initPointJA(&AJ, &ax, &ay, &az);
	initPointJA(&RJ, &rjx, &rjy, &rjz);
	initPoint(&RA, &rx, &ry);

	start = cpucycles();
	ECDBLJ(&RJ, &AJ, &P);
	end = cpucycles();
	JAECDBLcc += (end - start);
	J2A(&RA, &RJ, &P);

	//결과값 파일에 쓰기
	printf("DBLJ: ");
	fprintf(fp4, "DBLJ: ");
	for (int i = RA.x->top - 1; i >= 0; i--)
	{
		printf("%08x", RA.x->d[i]);
		fprintf(fp4, "%08x", RA.x->d[i]);
	}
	printf(", ");
	fprintf(fp4, ", ");
	for (int i = RA.y->top - 1; i >= 0; i--)
	{
		printf("%08x", RA.y->d[i]);
		fprintf(fp4, "%08x", RA.y->d[i]);
	}
	printf("\n\n");
	fprintf(fp4, "\n\n");
	
	//********************************//
	//테스트 벡터들 읽기
	fscanf(fp1, ")\n2. 2 x G = (");
	for (int i = 7; i >= 0; i--)
	{
		fscanf(fp1, "%08x", &opA[i]);
	}
	fscanf(fp1, ", ");
	for (int i = 7; i >= 0; i--)
	{
		fscanf(fp1, "%08x", &opB[i]);
	}
	initBignum(&opA, 8, &rjx);
	initBignum(&opB, 8, &rjy);
	initPointJA(&RJ, &rjx, &rjy, &rjz2);
	//********************************//

	//Jacobian addition
	start = cpucycles();
	ECADDJ(&RJ, &RJ, &A, &P);
	end = cpucycles();
	JAECADDcc += (end - start);
	J2A(&RA, &RJ, &P);

	//결과값 파일에 쓰기
	printf("ADDJ: ");
	fprintf(fp5, "ADDJ: ");
	for (int i = RA.x->top - 1; i >= 0; i--)
	{
		printf("%08x", RA.x->d[i]);
		fprintf(fp5, "%08x", RA.x->d[i]);
	}
	printf(", ");
	fprintf(fp5, ", ");
	for (int i = RA.y->top - 1; i >= 0; i--)
	{
		printf("%08x", RA.y->d[i]);
		fprintf(fp5, "%08x", RA.y->d[i]);
	}
	printf("\n\n");
	fprintf(fp5, "\n\n");

	printf("ECADDcc = %d\n", ECADDcc);
	printf("ECDBLcc = %d\n", ECDBLcc);
	printf("JAECADDcc = %d\n", JAECADDcc);
	printf("JAECDBLcc = %d\n", JAECDBLcc);

	fclose(fp0);
	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
	fclose(fp4);
	fclose(fp5);
}

void SCALARMUL()
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
	BIGNUM rx = { 0, };
	BIGNUM ry = { 0, };
	BIGNUM scalar = { 0, };

	BIGNUMPOINT A = { 0, };
	BIGNUMPOINT B = { 0, };
	BIGNUMPOINT R = { 0, };

	size_t len = 0;
	uint32_t count = 0;

	unsigned long long start = 0, end = 0;
	unsigned long long SMcc = 0;

	fp0 = fileOpen("P256값.txt", "r");
	fp1 = fileOpen("중간결과값.txt", "r");
	fp2 = fileOpen("TV_Scalar.txt", "r");
	fp3 = fileOpen("scalarMul.txt", "w");

	//p256값 저장
	fscanf(fp0, "%s", input_str);
	len = strlen(input_str) / (sizeof(int) * 2);
	str2hex(input_str, P256, len);
	initBignum(P256, len, &P);

	//테스트 벡터들 읽기
	fscanf(fp1, "1. 1 x G = (");
	for (int i = 7; i >= 0; i--)
	{
		fscanf(fp1, "%08x", &opA[i]);
	}
	fscanf(fp1, ", ");
	for (int i = 7; i >= 0; i--)
	{
		fscanf(fp1, "%08x", &opB[i]);
	}

	initBignum(&opA, 8, &ax);
	initBignum(&opB, 8, &ay);
	initPoint(&A, &ax, &ay);
	initPoint(&R, &rx, &ry);

	while (fscanf(fp2, "%08x", &opC[7]) != EOF)
	{
		count++;
		for (int i = 6; i >= 0; i--)
		{
			fscanf(fp2, "%08x", &opC[i]);
		}
		initBignum(&opC, 8, &scalar);		

		//SM
		start = cpucycles();
		LtRMul(&R, &scalar, &A, &P);
		end = cpucycles();
		SMcc += (end - start);

		//결과값 파일에 쓰기
		for (int i = R.x->top - 1; i >= 0; i--)
		{
			fprintf(fp3, "%08X", R.x->d[i]);
		}
		fprintf(fp3, "\n");
		for (int i = R.y->top - 1; i >= 0; i--)
		{
			fprintf(fp3, "%08X", R.y->d[i]);
		}
		fprintf(fp3, "\n\n");
	}
	printf("SMcc = %d\n", SMcc/count);

	fclose(fp0);
	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
}
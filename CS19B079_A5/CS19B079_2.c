/*
* Program for multiplication of A stored in row major order and B in column major order
* Name: KR Hariharan
* Roll No: CS19B079
* Date: Mar 18, 2021
*/
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<sys/time.h>

int A[1024][1024], B[1024][1024];
long C[1024][1024];

void rowRowMultiplication(int A[1024][1024], int B[1024][1024], long C[1024][1024])
{
	int i, j, k;
	long sum=0;
	for(i=0; i<1024; i++)
	{
		for(j=0; j<1024; j++)
		{
			for(k=0; k<1024; k++)
			{
				sum+=(long)A[i][k]*(long)B[j][k];
			}	
			C[i][j] = sum;
			sum=0;
		}
	}
	return;
}

int main()
{
	struct timeval current_time;
  	gettimeofday(&current_time, NULL);
  	printf("seconds : %ld\nmicro seconds : %ld\n", current_time.tv_sec, current_time.tv_usec);
	srand(time(NULL));
	int i, j;
	for(i=0; i<1024; i++)
	{
		for(j=0; j<1024; j++)
		{
			A[i][j] = rand() % 11;
			B[i][j] = rand() % 11;
		}
	}
	rowRowMultiplication(A, B, C);
  	gettimeofday(&current_time, NULL);
	printf("seconds : %ld\nmicro seconds : %ld\n", current_time.tv_sec, current_time.tv_usec);
	return 0;
}

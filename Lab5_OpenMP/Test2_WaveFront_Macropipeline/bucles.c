// Héctor Del Campo Pando

#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include <math.h>

#define SIZE	512
#define SEED	6834723

int main() {
	int i,j,k;

	omp_set_num_threads(4);

	int A[ SIZE ][ SIZE ];

	srand( SEED );

	for (i=0; i<SIZE; i++) {
		A[0][i] = rand() % 10;
	    A[i][0] = rand() % 10;
	}

	int t[SIZE];

	for (i=1; i<SIZE; i++) {
		#pragma omp parallel for
		for (j=1; j<SIZE; j++) // Copy from previous row in parallel way
			A[i][j] = A[i-1][j];

		for (j=0; j<log2(SIZE); j++) { //Prefix summatory - Tree reduction
			#pragma omp parallel shared(A, t, i, j) private(k) 
			{
				#pragma omp for
				for(k = 1<<j; k < SIZE; k++) 
					t[k] = A[i][k] + A[i][k - (1<<j)];
				
				#pragma omp for
				for(k = 1<<j; k < SIZE; k++)
					A[i][k] =  t[k];
			}
		}
	}

	/* Comprobaci�n de resultados */
	int checksum = 0.0;
	for (i=0; i<SIZE; i++)
		for (j=0; j<SIZE; j++)
			checksum += A[i][j];

	printf("Checksum: %d\n", checksum );

	return 0;
}

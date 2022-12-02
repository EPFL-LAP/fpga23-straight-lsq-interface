
/**

* jacobi-1d-imper.c: This file is part of the PolyBench/C 3.2 test suite.

*

*

* Contact: Louis-Noel Pouchet <pouchet@cse.ohio-state.edu>

* Web address: http://polybench.sourceforge.net

*/


#include <stdlib.h>
#include "jacobi_1d.h"



int jacobi_1d(inout_int_t A_1[N], inout_int_t A_2[N], inout_int_t B[N])
{

	int t, i, j;

	for (t = 0; t < TSTEPS; t++)
	{

		for (i = 1; i < N - 1; i++) {
		    B[i] = 3 * (A_1[i-1] + A_1[i] + A_1[i + 1]);
		}

		for (j = 1; j < N - 1; j++) {
		    A_2[j] = B[j];
		}

	}		

	return t;

}



#define AMOUNT_OF_TEST 1



int main(void){

    inout_int_t A[AMOUNT_OF_TEST][N];
    inout_int_t B[AMOUNT_OF_TEST][N];
    inout_int_t C[AMOUNT_OF_TEST][N];


    for(int i = 0; i < AMOUNT_OF_TEST; ++i){

        for(int j = 0; j < N; ++j){
            A[i][j] = rand()%100;
            B[i][j] = rand()%100;
            C[i][j] = rand()%100;
        }
	}



    //for(int i = 0; i < AMOUNT_OF_TEST; ++i){

        int i = 0;

        jacobi_1d(A[i], B[i], C[i]);

    //}

}

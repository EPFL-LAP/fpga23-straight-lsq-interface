#include "triangular.h"
//------------------------------------------------------------------------
// Triangular
//------------------------------------------------------------------------

int triangular( in_int_t x[100], inout_int_t A[100][100] , in_int_t n) {
	int i;
    for (i=n-1; i>=0; i--) {
       // x[i] = A[i][n]/A[i][i];
	    for (int k=i-1;k>=0; k--) {
            A[k][n] -= A[k][i] * x[i];
        }
	}

	return i + n;
}

#include <stdlib.h>

#define AMOUNT_OF_TEST 1

int main(void){
	in_int_t xArray[AMOUNT_OF_TEST][100];
	in_int_t A[AMOUNT_OF_TEST][100][100];
	in_int_t n[AMOUNT_OF_TEST];

	for(int i = 0; i < AMOUNT_OF_TEST; ++i){
		n[i] = 100; //(rand() % 100);
		for(int x = 0; x < 100; ++x){
            xArray[i][x] = rand() % 1000;
		    for(int y = 0; y < 10; ++y){
			    A[i][y][x] = rand() % 1000;
		    }
	    }
    }
    
	//for(int i = 0; i < AMOUNT_OF_TEST; ++i){
    int i = 0;
	triangular(xArray[i], A[i], n[i]);
	//}
}





/**
 * 2mm.c: This file is part of the PolyBench/C 3.2 test suite.
 *
 *
 * Contact: Louis-Noel Pouchet <pouchet@cse.ohio-state.edu>
 * Web address: http://polybench.sourceforge.net
 */

#include "kernel_2mm.h"
#include <stdio.h>
#include <stdlib.h>

//int kernel_2mm(in_int_t feature[S], inout_int_t hist[S], inout_int_t B[S])
int kernel_2mm(in_int_t alpha, in_int_t beta, inout_int_t tmp[N][N], in_int_t A[N][N], in_int_t B[N][N], in_int_t C[N][N], inout_int_t D[N][N])
{
    /*int sum = 2;
    for(int hh = 0; hh < 2; hh++) {
        for(int k = 0; k < 2; k++) {
            for(int i = 0; i < 2; i++) {
                int m = feature[i];
                hist[m] = hist[m] + i;
                B[m] = B[m] + i;
            }

            for(int j = 0; j < 2; j++) {
                int m = feature[j];
                int x = hist[m];
                sum+= x;//hist[m];
                hist[m] = j + x;
                B[m] = x * j;
            }
        }
    }

    for(int ll = 0; ll < 2; ll++) {
        int m = feature[ll];
        hist[m] = ll * 5;
    }
      return sum;
  */

 /*int i, j, k;

  for (i = 0; i < NI; i++)
   for (j = 0; j < NJ; j++)
      {
    tmp[i][j] = 0;
    for (k = 0; k < NK; ++k)
      tmp[i][j] += alpha * A[i][k] * B[k][j];
      }
  for (i = 0; i < NI; i++)
    for (j = 0; j < NL; j++)
      {
    D[i][j] *= beta;
    for (k = 0; k < NJ; ++k)
      D[i][j] += tmp[i][k] * C[k][j] * beta;
  }

  return k;*/


    int i, j, k;

    for (i = 0; i < NI; i++)
        for (j = 0; j < NJ; j++) {
            int x = tmp[i][j];
            for (k = 0; k < NK; ++k)
                x += alpha * A[i][k] * B[k][j];
            tmp[i][j] = x;
        }

    for (i = 0; i < NI; i++)
        for (j = 0; j < NL; j++) {
            int x = D[i][j]*beta;
            for (k = 0; k < NJ; ++k)
                x += tmp[i][k] * C[k][j];
            D[i][j] = x;
        }

    return k;
}



#define AMOUNT_OF_TEST 1

int main(void){
    /* in_int_t feature[AMOUNT_OF_TEST][S];
      in_int_t weight[AMOUNT_OF_TEST][S];
      inout_int_t hist[AMOUNT_OF_TEST][S];
      in_int_t n[AMOUNT_OF_TEST];

      inout_int_t B[AMOUNT_OF_TEST][S];

    srand(13);
    for(int i = 0; i < AMOUNT_OF_TEST; ++i){
    n[i] = S;
    for(int j = 0; j < S; ++j){
      feature[i][j] = rand()%1000;//(j*2)%S; //
      weight[i][j] = rand()%100;//(j+2)%S;//
      hist[i][j] = rand()%100;//(j+4)%S;//
      B[i][j] = rand()%100;//(j+4)%S;//
    }
    }

    //for(int i = 0; i < AMOUNT_OF_TEST; ++i){
        int i = 0; 
        kernel_2mm(feature[i], hist[i], B[i]);
    //} */
    in_int_t alpha[AMOUNT_OF_TEST];
    in_int_t beta[AMOUNT_OF_TEST];
    in_int_t tmp[AMOUNT_OF_TEST][N][N];
    in_int_t A[AMOUNT_OF_TEST][N][N];
    in_int_t B[AMOUNT_OF_TEST][N][N];
    in_int_t C[AMOUNT_OF_TEST][N][N];
    inout_int_t D[AMOUNT_OF_TEST][N][N];
    

    for(int i = 0; i < AMOUNT_OF_TEST; ++i){
        alpha[i] = rand();
        beta[i] = rand();
        for(int y = 0; y < N; ++y){
            for(int x = 0; x < N; ++x){
                A[i][y][x] = rand()%100;
                B[i][y][x] = rand()%100;
                C[i][y][x] = rand()%100;
                D[i][y][x] = rand()%100;
            }
        }
    }

    //for(int i = 0; i < AMOUNT_OF_TEST; ++i){
        int i = 0;
        kernel_2mm(alpha[0], beta[0], tmp[0], A[0], B[0], C[0], D[0]);
    //}
    

}



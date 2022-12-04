/**
 * atax.c: This file is part of the PolyBench/C 3.2 test suite.
 *
 *
 * Contact: Louis-Noel Pouchet <pouchet@cse.ohio-state.edu>
 * Web address: http://polybench.sourceforge.net
 */
#include "atax.h"

#define NX 20
#define NY 20
#define N 20

#include <stdlib.h>

#define AMOUNT_OF_TEST 1

int atax(in_float_t A[N][N], in_float_t x[N], inout_float_t y[N], inout_float_t tmp[N])
{
  int i, j;

  for (i = 0; i < NX; i++)
    {
      int t = tmp[i];
      for (j = 0; j < NY; j++)
         t = t + A[i][j] * x[j];
      for (j = 0; j < NY; j++)
         y[j] = y[j] + A[i][j] * t;
    tmp[i] = t; 
    }

    return i;

  /*int k,i,p;

  for(k=0; k<NTRU_N; k++)
  {
    r[k] = 0;
    for(i=1; i<NTRU_N-k; i++)
      r[k] += a[k+i] * b[NTRU_N-i];
    for(i=0; i<k+1; i++)
      r[k] += a[k-i] * b[i];

  p = i+k;
  }

  return p;*/
}

int main(void){
  /*out_int_t r[NTRU_N];
  in_int_t a[NTRU_N];
  in_int_t b[NTRU_N];

  for(int i = 0; i < NTRU_N; i++) {
    r[i] = 0;
    a[i] = i % 10;
    b[i] = (NTRU_N - i) % 10;
  }

  atax(r, a, b);*/
   in_float_t A[AMOUNT_OF_TEST][N][N];
    in_float_t x[AMOUNT_OF_TEST][N];
    inout_float_t y[AMOUNT_OF_TEST][N];
    inout_float_t tmp[AMOUNT_OF_TEST][N];
  
    
    srand(13);

    for(int i = 0; i < AMOUNT_OF_TEST; ++i){
        for(int j = 0; j < N; ++j){
            x[i][j] = rand()%100;
            y[i][j] = 0;
            tmp[i][j] = 0;
            for(int x = 0; x < N; ++x){
                A[i][j][x] = rand()%100;
            }
        }
    }

  //for(int i = 0; i < AMOUNT_OF_TEST; ++i){
    int i = 0;
        atax(A[0], x[0], y[0], tmp[0]);
  //}
}





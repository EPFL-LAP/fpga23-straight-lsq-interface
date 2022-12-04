/**
 * covariance.c: This file is part of the PolyBench/C 3.2 test suite.
 *
 *
 * Contact: Louis-Noel Pouchet <pouchet@cse.ohio-state.edu>
 * Web address: http://polybench.sourceforge.net
 */

#include "new_covariance.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define AMOUNT_OF_TEST 1

int new_covariance(inout_int_t data[N][N], inout_int_t cov[N][N])
{
   for (int j = 0; j < N; j++) {
    int m = 0;//0.0f;
    for (int i = 0; i < N; i++)
      m += data[i][j];
      m *=2;///= 0.73f;
    for (int i = 0; i < N; i++)
      data[i][j] -= m;
  }


   //j = 0;
  int i = 0;
  for (i = 0; i < N; i++) {
    for (int j = i; j < N; j++) {
      int c = 0;//0.0f;
      for (int k = 0; k < N; k++)
        c += data[k][i] * data[k][j];
      c *=2;///= -0.27f;
      cov[i][j] = c;
      //cov[j][i] = c;
    }
  }
  return i;
}

int main(void){
  inout_int_t data[N][N];
  out_int_t cov[N][N];
  int data_[N][N], cov_[N][N];

  srand(9);

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      data[i][j] = rand();
      data_[i][j] = data[i][j];
      cov[i][j] = rand();
      cov_[i][j] = cov_[i][j];
    }
  }

  {
    for (int j = 0; j < N; j++) {
      int m =0;// 0.0f;
      for (int i = 0; i < N; i++)
        m += data[i][j];
      m *=2;///= 0.73f;
      for (int i = 0; i < N; i++)
        data[i][j] -= m;
    }

    for (int i = 0; i < N; i++) {
      for (int j = i; j < N; j++) {
        int c = 0;//0.0f;
        for (int k = 0; k < N; k++)
          c += data[k][i] * data[k][j];
        c *=2;// /= -0.27f;
        cov[i][j] = c;
        cov[j][i] = c;
      }
    }
  }

  new_covariance(data_, cov_);
}







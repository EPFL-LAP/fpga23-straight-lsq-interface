//typedef int in_int_t;
//typedef int out_int_t;
//typedef int inout_int_t;

#include <stddef.h>
#include <stdint.h>

typedef uint32_t out_int_t; 
typedef uint32_t in_int_t;

typedef int in_float_t;
typedef int out_float_t;
typedef int inout_float_t;


#define N 20


int atax(in_float_t A[N][N], in_float_t x[N], inout_float_t y[N], inout_float_t tmp[N]);
//int atax(out_int_t r[32], in_int_t a[32], in_int_t b[32]);

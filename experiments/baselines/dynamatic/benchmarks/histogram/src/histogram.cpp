
//------------------------------------------------------------------------
// Histogram
//------------------------------------------------------------------------


#include <stdlib.h>
#include "histogram.h"

#define AMOUNT_OF_TEST 1

int histogram( in_int_t feature[1000], in_int_t weight[1000], inout_int_t hist[1000], in_int_t n ) 
{
  int i;
	for (i=0; i<n; ++i) {
    int m = feature[i];
    int wt = weight[i];
    int x = hist[m];
    hist[m] = x + wt;
  }
  return i;
}


int main(void){
	  in_int_t feature[AMOUNT_OF_TEST][1000];
	  in_int_t weight[AMOUNT_OF_TEST][1000];
	  inout_int_t hist[AMOUNT_OF_TEST][1000];
	  in_int_t n[AMOUNT_OF_TEST];
    
	srand(13);
	for(int i = 0; i < AMOUNT_OF_TEST; ++i){
    n[i] = 1000;
    for(int j = 0; j < 1000; ++j){
      feature[i][j] = rand()%1000;
      weight[i][j] = rand()%100;
      hist[i][j] = rand()%100;
    }
	}

	//for(int i = 0; i < AMOUNT_OF_TEST; ++i){
		int i = 0; 
		histogram(feature[i], weight[i], hist[i], n[i]);
	//}
}



//------------------------------------------------------------------------
// Histogram
//------------------------------------------------------------------------


#include <stdlib.h>
#include "histogram.h"

#define AMOUNT_OF_TEST 1

int histogram(in_int_t feature[1000], in_int_t weight[1000], inout_int_t hist[1000], in_int_t n) 
{
	int i;
	for (i=0; i<n; ++i) {
    int m = feature[i];
    int wt = weight[i];
    int x = hist[m];
    hist[m] = x + wt;
  }
  return i;
	/*for(int i = 0; i < 5; i++) {
		int m = feature[i];
		hist[m] = i;
	}

	int sum = 0;
	for (int j = 0; j < 5; j++)
	{
		sum += hist[j];
	}

	return sum + n;*/

	/*int x;
	for(int i = 0; i < n; i++) {
		int m = feature[i];
		x = hist[m];
		hist[i] = x * i;
	}
	return x;*/

  /*int m;
  for (int i=0; i<n; ++i) {
    m = feature[i];
    int wt = weight[i]; // was float
    int x = hist[m]; // was float
    hist[m] = x + wt;
  }

  return m; */

	/*int sum = 0;
	//for(int k = 0; k < 2; k++) {
		for(int i = 0; i < 10; i++) {
			int m = feature[i];
			//sum+= hist[m];
			hist[m] = hist[m] + i;
		}

		for(int j = 0; j < 10; j++) {
			int m = feature[j];
			sum+= hist[m];
			hist[m] = j;
		}

	//}
  
  return sum;*/

	/*int sum = 2;
	for(int m = 0; m < 2; m++) {
		for(int k = 0; k < 2; k++) {
			for(int i = 0; i < 2; i++) {
				int m = feature[i];
				hist[m] = hist[m] + i;
			}

			for(int j = 0; j < 2; j++) {
				int m = feature[j];
				int x = hist[m];
				sum+= x;//hist[m];
				hist[m] = j + x;
			}
		}
	}
  
  return sum; */

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


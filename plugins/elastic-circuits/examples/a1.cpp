//------------------------------------------------------------------------
// My customized example
//------------------------------------------------------------------------

int a_1(int a, int b)
{
	/*int i,j,k;
	int tmp = 0;
	for (i = 0; i < 30; i++)
    	for (j = 0; j < 30; j++)
		  {
		  	//tmp = a *beta;
			for (k = 0; k < 30; ++k)
				tmp += alpha * a * b;

			// C[i][j] = tmp;
		  }

	return tmp;*/

	int c = 0;
	int sum = a + b;

	// Example of the case where the consumer is in a loop that the producer isn't in
	for(int i = 0; i < 10; i++) {
		//for(int j = 0; j < 50; j++) {
			c += sum * i;
		//} 
	}

	return c;
	
	
	/*int c = 0;
	if(a < b) {
		c = 2 * a + b;
		if(c > 3) {
			c = 1;
		} else {
			c = c + 1;
		}
	} else {
		c = a * 4; 
	}

	return c;
	*/

	/*for(int i = 0; i < 1000; i++){
		val_1 += a[i];  // bb1
	}
	val_arr[0] = val_1;  //
	if(flag == 1){  // bb1
		out_1[0] = 2;	// bb2
	}else{
		out_1[0] = 3;	// bb3
		
		for(int i = 0; i < 1000; i++){
			val_2 += a[i];  // bb4
		}

		if(flag == 1){
			out_2[0] = 2; // bb5
		}else{
			out_2[0] = val_arr[0] * 3; // 
		}
	}*/
}

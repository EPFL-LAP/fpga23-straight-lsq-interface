//------------------------------------------------------------------------
// My customized example
//------------------------------------------------------------------------

int a_2(int a)
{
	/*int sum = 0;
	for(int i = 0; i < 50; i++) {
		//sum = a+i;
		for(int j = 0; j < 10; j++) {
			sum += i*j;
		}
	}
	
	return sum + a;*/

	int sum_1 = 0;
	int sum_2 = 0;
	int sum_3 = 0;

	for(int i = 0; i < 50; i++) {
		for(int j = 0; j < 3; j++) {
			sum_1 += i*j;
		}
	}

	for(int k = 0; k < 1000; k++) {
		//for(int l = 0; l < 300; l++) {
			sum_2 += k;
		//}
	}

	if(a > 3) {
		sum_3 = sum_1;
	} else {
		sum_3 = sum_2;
	}

	return sum_3; 
}

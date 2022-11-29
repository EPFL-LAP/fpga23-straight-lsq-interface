//------------------------------------------------------------------------
// My customized example
//------------------------------------------------------------------------

int a_2(int a)
{
	int sum_1 = 0;
	int sum_2 = 0;
	//int sum_3 = 0;

	for(int i = 0; i < 50; i++) {
		//for(int j = 0; j < 10; j++) {
			sum_1 += i;
		//}
	}

	for(int g = 0; g < 100; g++) {
		//for(int k = 0; k < 100; k++) {
			sum_2 += g;
		//}
	}

	/*for(int i = 0; i < 50; i++) {
		sum_3 = a;
		for(int j = 0; j < 10; j++) {
			sum_3 += i*j;
		}
	}
	
	return sum_1 + sum_2 + sum_3 + a;*/

	return sum_1 + sum_2+ a;

}

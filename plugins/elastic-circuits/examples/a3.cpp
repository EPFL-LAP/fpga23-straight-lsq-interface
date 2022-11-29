//------------------------------------------------------------------------
// My customized example
//------------------------------------------------------------------------

int a_2(int alpha, int beta, int A[30][30], int B[30][30], int C[30][30] )
{
	 int tmp = 0;
  //int a;

 //for (i = 0; i < 30; i++)
    for (int j = 0; j < 30; j++)
	{
		//for (int k = 0; k < 30; k++)
			tmp += alpha*j;// * A[k] * B[k];
	}

	return tmp;

}

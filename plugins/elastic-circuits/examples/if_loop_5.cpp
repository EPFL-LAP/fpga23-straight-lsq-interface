//------------------------------------------------------------------------
// If loop
//------------------------------------------------------------------------

int if_loop(int a[1000], int b[1000]) {
    int i;
    int dist;
    int sum = 1;

    for (i = 0; i < 1000; i++) {
        dist = a[i] - b[i];

        if (dist >= 0) {

            sum = (sum / dist);
        }
    }
    return sum;
}

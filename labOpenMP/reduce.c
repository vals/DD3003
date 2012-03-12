/**********************************************************************
 * Global reduction
 *
 **********************************************************************/

#include <omp.h>
#include <stdio.h>

int main(int argc, char *argv[]) {

  int rank, size, i;
  const int n = 1000000L ;
  double sum, globsum;
  double A[1000000];

  for (i = 0; i < n ; i++) { 
    A[i]=(double)i;
  }

  sum=0.0;

#pragma omp parallel for reduction(+:sum)
    for (i = 0; i < n ; i++) { 
      sum = sum + A[i];
    }


  printf("Global sum is: %f\n",  sum);

  return 0;
}

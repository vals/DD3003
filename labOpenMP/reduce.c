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


  globsum=0.0;
#pragma omp parallel private(sum)
  {
    sum=0.0;

#pragma omp for
    for (i = 0; i < n ; i++) { 
      sum += A[i];
    }

#pragma omp critical
    {
      globsum+=sum;
    }
  }

  printf("Global sum is: %f\n",  globsum);

  return 0;
}

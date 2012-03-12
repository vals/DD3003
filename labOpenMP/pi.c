/**********************************************************************
 * This program calculates pi using OpenMP/C
 *
 **********************************************************************/

#include <omp.h>
#include <stdio.h>

int main(int argc, char *argv[]) {

  int rank, size, i;
  const int intervals = 100000000L ;
  double sum, dx, x;

  dx  = 1.0/intervals;
  sum = 0.0;

#pragma omp parallel for reduction(+:sum) private(x)
  for (i = 1; i <= intervals; i++) { 
    x = dx*(i - 0.5);
    sum += dx*4.0/(1.0 + x*x);
  }

  printf("PI is approx. %.16f\n",  sum);

  return 0;
}

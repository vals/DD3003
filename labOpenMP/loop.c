/**********************************************************************
 * Example: Loop parallelism in OpenMP/C
 *
 *
 **********************************************************************/

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int timer(void);

double work(double c){
  double d=0.0;

  while (d<c){
    d=d+1.0;
  }
  return d;
}    

int main(int argc, char *argv[]) {
  int i,n=100000;
  double *A,*B;
  int timer1;
 
  A = (double *)malloc(n*sizeof(double));
  B = (double *)malloc(n*sizeof(double));
  for (i=0;i<n;i++){
    A[i]=(double)i;
  }

  timer1=timer(); /*omp_get_wtime();*/
#pragma omp parallel
  {
#pragma omp for 
    for (i=0;i<n;i++){
      B[i]=work(A[i]);
    }
  }
  timer1=timer()-timer1;
  printf("Time: %f sec\n",timer1/1000000.0);

  return 0;
}

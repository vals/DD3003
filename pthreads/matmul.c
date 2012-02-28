#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

double **A,**B,**C;
int n;

int main(int argc, char *argv[]) {
   int i,j,k,time;
   double t;

   n = atoi(argv[1]);
  
   //Allocate and fill matrices
   A = (double **)malloc(n*sizeof(double *));
   B = (double **)malloc(n*sizeof(double *));
   C = (double **)malloc(n*sizeof(double *));
   for(i=0;i<n;i++){
      A[i] = (double *)malloc(n*sizeof(double));
      B[i] = (double *)malloc(n*sizeof(double));
	  C[i] = (double *)malloc(n*sizeof(double));
      }

  for (i = 0; i<n; i++)
	  for(j=0;j<n;j++){
		  A[i][j] = rand() % 5 + 1;
		  B[i][j] = rand() % 5 + 1;
		  C[i][j] = 0.0;
	  }
    
   time=timer();
		  
   // Multiply C=A*B		  
   for(i=0;i<n;i++)
	   for (j=0;j<n;j++)
		   for (k=0;k<n;k++)
			   C[i][j]+=A[i][k]*B[k][j];
		  
   time=timer()-time;
   printf("Elapsed time: %f \n",time/1000000.0);

   return 0;
   
}

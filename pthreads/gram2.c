#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <pthread.h>
#define NUM_THREADS	2

double **V,**Q;
int n;

struct thread_data
{
	int	iter;
	int j1;
	int j2;
};

struct thread_data thread_data_array[NUM_THREADS];

void *gs(void *threadarg);
double vecNorm(double *,int );
double scalarProd(double *,double *,int);


int main(int argc, char *argv[]) {
   int i,j,k,time,t;
   double sigma,temp_norm,norm;
	void *status;
	pthread_t thread[NUM_THREADS];
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

   n = atoi(argv[1]);
  
   //Allocate and fill vectors
   V = (double **)malloc(n*sizeof(double *));
   Q = (double **)malloc(n*sizeof(double *));
   for(i=0;i<n;i++){
      V[i] = (double *)malloc(n*sizeof(double));
      Q[i] = (double *)malloc(n*sizeof(double));
      }

  for (i = 0; i<n; i++)
	  for(j=0;j<n;j++)
		  V[i][j] = rand() % 5 + 1;
    
	time=timer();
   for(i=0;i<n;i++){
	   
	   temp_norm = vecNorm(V[i],n);
	   for (k=0; k<n; k++)
		   Q[i][k] = V[i][k]/temp_norm;

	   for(t=0; t<NUM_THREADS-1; t++) {
		   thread_data_array[t].iter=i;
		   thread_data_array[t].j1=i+1+(n-i-1)/NUM_THREADS*t;
		   thread_data_array[t].j2=i+1+(n-i-1)/NUM_THREADS*(t+1);
		   // printf("%d %d %d\n",t,thread_data_array[t].j1,thread_data_array[t].j2);
		   pthread_create(&thread[t], &attr, gs, (void *)&thread_data_array[t]);
		  
	   }
	   t=NUM_THREADS-1;
	   thread_data_array[t].iter=i;
	   thread_data_array[t].j1=i+1+(n-i-1)/NUM_THREADS*t;
	   thread_data_array[t].j2=n;
	   //printf("%d %d %d\n",t,thread_data_array[t].j1,thread_data_array[t].j2);
       gs((void *)&thread_data_array[t]);
	   
	   for(t=0; t<NUM_THREADS-1; t++)
		   pthread_join(thread[t], &status);
   }

	time=timer()-time;
   printf("Elapsed time: %f \n",time/1000000.0);
	printf("Check orthogonality: %e \n",scalarProd(Q[n/2], Q[n/3], n));
	
   return 0;
   
}

void *gs(void *threadarg){
	int k,j,j1,j2,i;
	double sigma;
	struct thread_data *my_data;
	my_data = (struct thread_data *) threadarg;
	j1=my_data->j1; j2=my_data->j2; i=my_data->iter;
	
	for(j=j1;j<j2;j++){
		sigma = scalarProd(Q[i],V[j],n);
		for(k=0;k<n;k++)
			V[j][k] -=sigma*Q[i][k];
	}
}

double vecNorm(double *vec,int n){
   int i;
   double local_norm = 0;
   
   for(i=0;i<n;i++){
      local_norm+= (vec[i]*vec[i]);
   }
   return sqrt(local_norm);
}

double scalarProd(double *a,double *b,int n){
   int i;
   double scalar =0.0;
   
   for(i=0;i<n;i++){
      scalar +=a[i]*b[i];
   }
   
   return scalar;
}

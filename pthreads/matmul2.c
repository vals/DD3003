/**********************************************************************
 * This program multiplies matrices (with multiple threads)
 *
 **********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include <time.h>

#define NUM_THREADS     10

typedef struct a1 {

  int start;
  int end;
  int total;

} tparam;

double **A,**B,**C;

int timer(void)
{
  struct timeval tv;
  gettimeofday(&tv, (struct timezone*)0);
  return (tv.tv_sec*1000000+tv.tv_usec);
}

void *thread_func(void *arg) {

  int i, j, k;
  tparam *t;

  t = (tparam *) arg;

  // Multiply C=A*B
   for (i = t->start; i < t->end; i++)
           for (j = 0; j < t->total; j++)
                   for (k = 0; k < t->total; k++)
                           C[i][j]+=A[i][k]*B[k][j];

  pthread_exit(NULL);

}

int main(int argc, char *argv[]) {

  pthread_t thread[NUM_THREADS];
  int i, j, rc, n, time, end, start, thread_loop;
  tparam params[NUM_THREADS];

  if (argc != 2) {
     printf("\nUSAGE: ./matmul2 [row/column size]\n\n");
     exit(-1);
  }

  n = atoi(argv[1]);

  if ((n % NUM_THREADS) != 0) {
      printf("\n'NUM_THREADS' is not even divisible by %d!!!\n\n", n);
      exit(-1);
  }

  //Allocate and fill matrices
  A = (double **)malloc(n*sizeof(double *));
  B = (double **)malloc(n*sizeof(double *));
  C = (double **)malloc(n*sizeof(double *));

  for(i = 0; i < n; i++) {
     A[i] = (double *)malloc(n*sizeof(double));
     B[i] = (double *)malloc(n*sizeof(double));
     C[i] = (double *)malloc(n*sizeof(double));
  }

  for (i = 0; i < n; i++) {
     for (j = 0; j < n; j++){
        A[i][j] = rand() % 5 + 1;
        B[i][j] = rand() % 5 + 1;
        C[i][j] = 0.0;
     }
  }


  time=timer();

  thread_loop = n/NUM_THREADS;
  start = 0;
  end = thread_loop;

  // Divide the workload among threads
  for (i = 0; i < NUM_THREADS; i++) {
     params[i].start = start;
     params[i].end = end;
     params[i].total = n;

     start += thread_loop;
     end += thread_loop;
  }

  for (i = 0; i < NUM_THREADS; i++) {

      rc = pthread_create(&thread[i], NULL, thread_func, (void *)&params[i]);
      if (rc) {
         printf("ERROR; return code from pthread_create() is %d\n", rc);
         exit(-1);
      }
  }

  for (i = 0; i < NUM_THREADS; i++) {
      rc = pthread_join(thread[i], NULL);
      if (rc) {
         printf("ERROR; return code from pthread_join() is %d\n", rc);
         exit(-1);
      }
  }

  time=timer()-time;

  printf("\nElapsed time: %f \n\n",time/1000000.0);

  return 1;
}

/**********************************************************************
 * This program calculates pi using C (with multiple threads)
 *
 **********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#define NUM_THREADS     32

typedef struct a1 {

  int id;
  int start;
  int end;

} tparam;

double results[NUM_THREADS];

const int intervals = 100000000L;
double sum;
double dx;

void *thread_func(void *arg) {

  double x, local_sum;
  int i;
  tparam *t;

  t = (tparam *) arg;

  local_sum = 0.0;

  for (i = t->start; i <= t->end; i++) {
    x = dx*(i - 0.5);
    local_sum += dx*4.0/(1.0 + x*x);
  }

  results[t->id] = local_sum;

  pthread_exit(NULL);

}

int main(int argc, char *argv[]) {

  pthread_t thread[NUM_THREADS];
  int i, rc, thread_loop, start, end, utime;
  tparam params[NUM_THREADS];
  struct timeval tv1, tv2;

  dx  = 1.0/intervals;
  sum = 0.0;

  if ((intervals % NUM_THREADS) != 0) {
      printf("\n'NUM_THREADS' is not even divisible by 100000000!!!\n\n");
      exit(-1);
  }

  gettimeofday(&tv1, NULL);

  thread_loop = intervals/NUM_THREADS;
  start = 1;
  end = thread_loop;

  // Divide the workload among threads
  for (i = 0; i < NUM_THREADS; i++) {
    params[i].start = start;
    params[i].end = end;
    params[i].id = i;

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

  // Sum the results from all threads
  for (i = 0; i < NUM_THREADS; i++) {
    sum += results[i];
  }

  gettimeofday(&tv2, NULL);

  printf("PI is approx. %.16f\n\n",  sum);

  utime = (((tv2.tv_sec*1000000)+tv2.tv_usec)-((tv1.tv_sec*1000000)+tv1.tv_usec));
  printf("TOTAL TIME: %d micro sec (%f milli sec)\n\n", utime, ((double)utime/1000));

  pthread_exit(NULL);
}

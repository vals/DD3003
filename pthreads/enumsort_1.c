/**********************************************************************
 * Enumeration sort
 *
 **********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define NUM_THREADS	2
#define len 100000

double indata[len], outdata[len];

void *findrank(void *arg)
{
	int rank,i;
	long j=(long)arg;
	
	rank=0;
	for (i=0;i<len;i++)
		if (indata[i]<indata[j]) rank++;
	outdata[rank]=indata[j];
	pthread_exit(NULL);
}


int main(int argc, char *argv[]) {
	
  pthread_t threads[NUM_THREADS];
  pthread_attr_t attr;
  int seed,i,j,rank,nthreads,ttime,t;
  long el;
  void *status;
  
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	

  // Generate random numbers (Wichmann-Hill)
  seed=171;
  for (i=0;i<len;i++){
    seed=(171*seed)%30269;
    indata[i]=seed/30268.0;
    outdata[i]=-1;
  }


  // Enumeration sort
  ttime=timer();
  for (j=0;j<len;j+=NUM_THREADS)
    {
		for(t=0; t<NUM_THREADS; t++) {
			el=j+t;
		    pthread_create(&threads[t], &attr, findrank, (void *)el); }
		
		for(t=0; t<NUM_THREADS; t++) 
			pthread_join(threads[t], &status);
    }
  ttime=timer()-ttime;
  printf("Time: %f %d\n",ttime/1000000.0,NUM_THREADS);

  // Check results, -1 implies data same as the previous element
    for (i=0; i<len-1; i++)
      if (outdata[i]>outdata[i+1] && outdata[i+1]>-1)
	printf("ERROR: %f,%f\n", outdata[i],outdata[i+1]);

  return 0;
}

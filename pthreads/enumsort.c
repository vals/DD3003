/**********************************************************************
 * Enumeration sort
 *
 **********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define len 100000

double indata[len], outdata[len];

int main(int argc, char *argv[]) {

  int seed,i,j,rank,nthreads,ttime;

  // Generate random numbers (Wichmann-Hill)
  seed=171;
  for (i=0;i<len;i++){
    seed=(171*seed)%30269;
    indata[i]=seed/30268.0;
    outdata[i]=-1;
  }


  // Enumeration sort

  ttime=timer();
  for (j=0;j<len;j++)
    {
      rank=0;
      for (i=0;i<len;i++)
	if (indata[i]<indata[j]) rank++;
      outdata[rank]=indata[j];
    }
  ttime=timer()-ttime;
  printf("Time: %f\n",ttime/1000000.0);

  // Check results, -1 implies data same as the previous element
    for (i=0; i<len-1; i++)
      if (outdata[i]>outdata[i+1] && outdata[i+1]>-1)
	printf("ERROR: %f,%f\n", outdata[i],outdata[i+1]);

  return 0;
}

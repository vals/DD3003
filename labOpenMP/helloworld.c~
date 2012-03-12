/**********************************************************************
 * A simple "hello world" program for OpenMP/C
 *
 *
 **********************************************************************/

#include <omp.h>
#include <stdio.h>

int main(int argc, char *argv[]) {

  printf("Starting on %i threads\n", omp_get_max_threads());
 
#pragma omp parallel 
  {
  printf("Hello World from thread %i!\n", omp_get_thread_num()); 
  }

  printf("Stopping\n"); 

  return 0;
}

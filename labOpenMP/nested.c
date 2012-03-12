#include <omp.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {
  omp_set_nested(1);
  omp_set_num_threads(2);
  printf("Master: Nthr %d   Thrid %d   Nested %d\n",omp_get_num_threads(),omp_get_thread_num(),omp_get_nested());
#pragma omp parallel
  {
  printf("Parallel 1: Nthr %d   Thrid %d   Nested %d\n",omp_get_num_threads(),omp_get_thread_num(),omp_get_nested());
  omp_set_num_threads(2);
#pragma omp parallel
  {
  printf("Parallel 2: Nthr %d   Thrid %d   Nested %d\n",omp_get_num_threads(),omp_get_thread_num(),omp_get_nested());
  }
  }
}

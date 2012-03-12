/**********************************************************************
 * Example: Datasharing in OpenMP/C
 *
 *
 **********************************************************************/

#include <omp.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  int a,b;
  a=100;b=10;
 
#pragma omp parallel private(a)
  {
    a+=b;
    printf("a= %d b= %d\n",a,b);
  }

  return 0;
}

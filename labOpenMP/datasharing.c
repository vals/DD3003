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
 
#pragma omp parallel firstprivate(a)
  {
    a+=b;
    printf("a= %d b= %d\n",a,b);
  }

	printf("And now a is %d again!\n", a);
	return 0;
}

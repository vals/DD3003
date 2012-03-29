#include <math.h>
#include <stdlib.h>

#define leg 1024*1024

int main()
{
  int i;
  float *data;
  data = (float*)malloc(sizeof(float)*leg);
  for (i=0; i<leg; i++)
    data[i] = i;

  for (i=0;i<leg;++i) {
    double a=cos(data[i]);
  }
 
}		 

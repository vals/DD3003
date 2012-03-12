#include <stdio.h>
#include <sys/time.h>
#include <time.h>
 
int timer(void)
{
  struct timeval tv;
  gettimeofday(&tv, (struct timezone*)0);
  return (tv.tv_sec*1000000+tv.tv_usec);
}

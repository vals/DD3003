#ifndef barrier_h
#define barrier_h
#include <pthread.h>
#include <stdio.h>

class thread_barrier{
private:
  pthread_mutex_t lock;
  pthread_cond_t signal;
  int nthreads;
  int waiting;
  int ready;
  int step;
  int phase;
	
/* Peer-to-peer synch */
	pthread_mutex_t lock1,lock2;
	pthread_cond_t produce; 
	pthread_cond_t consume;
	pthread_t *left;
	pthread_t *right;
	int ready1,ready2;
	int id;

public:
  thread_barrier(int n);
  thread_barrier();
  thread_barrier(pthread_t *th1, pthread_t *th2, int i);
  ~thread_barrier();
  void init(pthread_t *th1, pthread_t *th2,int i);
  void setpeer();
  void waitpeer();
  int checkpeer();	
  void start(int &locstep);
  void end(int &locstep);
  void wait();
};
#endif

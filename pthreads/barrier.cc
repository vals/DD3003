#include "barrier.h"

thread_barrier::thread_barrier(int n)
{
	pthread_mutex_init(&lock,NULL);
	pthread_cond_init(&signal,NULL);
	pthread_mutex_init(&lock1,NULL);
	pthread_mutex_init(&lock2,NULL);
	pthread_cond_init(&produce,NULL);
	pthread_cond_init(&consume,NULL);
	nthreads=n;
	ready=0;
	step=0;
	phase=0;
	waiting=0;
}

thread_barrier::thread_barrier()
{
	pthread_mutex_init(&lock1,NULL);
	pthread_mutex_init(&lock2,NULL);
	pthread_cond_init(&produce,NULL);
	pthread_cond_init(&consume,NULL);
	pthread_mutex_init(&lock,NULL);
	pthread_cond_init(&signal,NULL);
	left=NULL; right=NULL;
	ready1=ready2=0;
}

thread_barrier::thread_barrier(pthread_t *th1, pthread_t *th2,int i)
{
	left=th1; right=th2;
	ready1=ready2=0;id=i;
}

thread_barrier::~thread_barrier()
{
	pthread_mutex_destroy(&lock);
	pthread_cond_destroy(&signal);
	pthread_mutex_destroy(&lock1);
	pthread_mutex_destroy(&lock2);
	pthread_cond_destroy(&produce);
	pthread_cond_destroy(&consume);
}

/* Start checkpoint */
void thread_barrier::start(int &locstep)
{
	pthread_mutex_lock(&lock);
	ready++; locstep++;
	if (ready==nthreads){
		ready=0;
		step++;
		pthread_cond_broadcast(&signal);
	}
	pthread_mutex_unlock(&lock);
}

/* End checkpoint */
void thread_barrier::end(int &locstep)
{
  pthread_mutex_lock(&lock);
	while (locstep>step)
		pthread_cond_wait(&signal,&lock);
  pthread_mutex_unlock(&lock);
}

void thread_barrier::wait()
{
  int myphase;
  pthread_mutex_lock(&lock);
  myphase=phase;
  waiting++;
  if (waiting==nthreads){
    waiting=0;
    phase=1-myphase;
	pthread_cond_broadcast(&signal);
  }
  while (phase==myphase)
	  pthread_cond_wait(&signal,&lock);
  pthread_mutex_unlock(&lock);
}

void thread_barrier::init(pthread_t *th1, pthread_t *th2, int i)
{
	left=th1; right=th2; id=i;
}

void thread_barrier::setpeer()
{
	if (pthread_equal(*left,*right)) return;
	if (pthread_equal(pthread_self(),*left)){
		pthread_mutex_lock(&lock1);
		while (ready1==1) // Wait until right proc consumes data
			pthread_cond_wait(&consume,&lock1);
		ready1=1;
		pthread_cond_signal(&produce);
		pthread_mutex_unlock(&lock1);
	}
	else if (pthread_equal(pthread_self(),*right)){
		pthread_mutex_lock(&lock2);
		while (ready2==1)
			pthread_cond_wait(&consume,&lock2);
		ready2=1;
		pthread_cond_signal(&produce);
		pthread_mutex_unlock(&lock2);
	}
	else
		printf("Thread error: setready!\n");
}

int thread_barrier::checkpeer()
{
	int ret;
	if (pthread_equal(*left,*right)) return 1;
	if (pthread_equal(pthread_self(),*left)){
		pthread_mutex_lock(&lock2);
	    ret=ready2;
	    pthread_mutex_unlock(&lock2);
	}
	else if (pthread_equal(pthread_self(),*right)){
		pthread_mutex_lock(&lock1);
	    ret=ready1;
	    pthread_mutex_unlock(&lock1);
	}
	return ret;
}


void thread_barrier::waitpeer()
{
	if (pthread_equal(*left,*right)) return;
	if (pthread_equal(pthread_self(),*left)){
		pthread_mutex_lock(&lock2);
		while (!ready2){
			pthread_cond_wait(&produce,&lock2);
		}
		ready2=0;
		pthread_cond_signal(&consume);
		pthread_mutex_unlock(&lock2);
	}
	else if (pthread_equal(pthread_self(),*right)){
		pthread_mutex_lock(&lock1);
		while (!ready1){
			pthread_cond_wait(&produce,&lock1);
		}
		ready1=0;
		pthread_cond_signal(&consume);
		pthread_mutex_unlock(&lock1);
	}
	else 
		printf("Thread error: wait!\n");
}


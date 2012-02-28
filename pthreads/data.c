#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM_THREADS	5
#define Nsize 5

/* Global data directly accessible to all threads */
int GlobData[Nsize];
pthread_mutex_t order;

struct thread_arg
{
	int tid;
	int *data;
};

void *work(void *arg)
{
	/* Local data only accessible to calling thread */
	int LocData,sum=0;
	
	struct thread_arg *myarg=(struct thread_arg *) arg;
	int tid=myarg->tid;
	int *Gdata=myarg->data;
	int t,i;
	
	/* Read global data in to cache */
	for (i=0;i<NUM_THREADS; i++) sum=sum+GlobData[i];
	
	/* Modify global data */
	GlobData[tid]=-tid;
	Gdata[tid]=tid;
	LocData=1000+tid;	
		
	/* Print global data */
	pthread_mutex_lock (&order);
	printf("Thread %d, with sum %d\n",tid,sum);
	printf("Thread %d, with LocData %d\n",tid,LocData);
	printf("Thread %d, with GlobData: ",tid);
	for (i=0;i<NUM_THREADS; i++) printf("%d ",GlobData[i]);
	printf("\n\n");
	pthread_mutex_unlock (&order);
	
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	/* Global data but needs to be passed to the threads */
	int *Data;
	
	struct thread_arg argarr[NUM_THREADS];
	pthread_t threads[NUM_THREADS];
	int	t;
	pthread_mutex_init(&order, NULL);
	
	/* Initialize data */
	Data=(int *)malloc(Nsize*sizeof(int));
	printf("Master begin:\n");
	for(t=0;t<Nsize;t++){
	Data[t]=-1; GlobData[t]=1;
	printf("Data %d, GlobData %d\n",Data[t],GlobData[t]);}
	printf("\n");
	
	/* Do something on the threads using global data*/
	for(t=0;t<NUM_THREADS;t++){
		argarr[t].tid=t; argarr[t].data=Data;
		pthread_create(&threads[t], NULL, work, (void *)&argarr[t]);
	}	
	for(t=0; t<NUM_THREADS; t++) 
		pthread_join(threads[t], NULL);
	
	/* Print the result on master thread */
	printf("Master end:\n");
	for(t=0;t<Nsize;t++)
		printf("Data %d, GlobData %d\n",Data[t],GlobData[t]);

	free(Data);	 // Do not free without joining threads
	pthread_exit(NULL);
}

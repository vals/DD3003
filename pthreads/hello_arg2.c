/******************************************************************************
* FILE: hello_arg2.c
* DESCRIPTION:
*   A "hello world" Pthreads program which demonstrates another safe way
*   to pass arguments to threads during thread creation.  In this case,
*   a structure is used to pass multiple arguments.
*
******************************************************************************/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM_THREADS	4

struct thread_data
{
    int	thread_id;
	char *message;
	int retpar;
};


void *PrintHello(void *threadarg)
{
	int taskid;
   char *hello_msg;
   struct thread_data *my_data;

   my_data = (struct thread_data *) threadarg;
   taskid = my_data->thread_id;
   hello_msg = my_data->message;
	
   printf("Thread %d: %s\n", taskid, hello_msg);
   my_data->retpar=-taskid;
   
   pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
struct thread_data thread_data_array[NUM_THREADS];
pthread_t threads[NUM_THREADS];
void* status;
	
int t;
char *messages[8];
	  messages[0] = "English: Hello World!";
	  messages[1] = "French: Bonjour, le monde!";
	  messages[2] = "Spanish: Hola al mundo";
	  messages[3] = "Klingon: Nuq neH!";
	  messages[4] = "German: Guten Tag, Welt!"; 
	  messages[5] = "Russian: Zdravstvytye, mir!";
	  messages[6] = "Japan: Sekai e konnichiwa!";
	  messages[7] = "Latin: Orbis, te saluto!";	

for(t=0;t<NUM_THREADS;t++){
  thread_data_array[t].thread_id = t;
	thread_data_array[t].message = "Hello World";
  pthread_create(&threads[t], NULL, PrintHello, (void *)&thread_data_array[t]);
  }
	
for(t=0;t<NUM_THREADS;t++){
	pthread_join(threads[t], &status);
	printf("Thread %d,  return value: %d\n",t,thread_data_array[t].retpar);
    }
	
pthread_exit(NULL);
}

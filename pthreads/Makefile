CPP=g++
CC=gcc
LIBS=-lm 
FLAGS= -O3 -pthread
wave: func.o wave.o time.cc wave.h barrier.o leapfrog.o
	$(CPP) $(FLAGS) -o wave wave.o time.cc barrier.o \
	func.o leapfrog.o $(LIBS)

wave.o: func.cc leapfrog.cc wave.cc wave.h
	$(CPP) $(FLAGS) -c wave.cc

pi: pi.c 
	$(CC) $(FLAGS) -o pi pi.c $(LIBS)
	
data: data.c 
	$(CC) $(FLAGS) -o data data.c $(LIBS)	
	
synch: synch.c 
	$(CC) $(FLAGS) -o synch synch.c $(LIBS)
		
spinwait: spinwait.c 
	$(CC) $(FLAGS) -o spinwait spinwait.c $(LIBS)
			
gram1: gram1.c time.c
	$(CC) $(FLAGS) -o gram1 gram1.c time.c $(LIBS)

gram2: gram2.c time.c
	$(CC) $(FLAGS) -o gram2 gram2.c time.c $(LIBS)

gram3: gram3.c time.c
	$(CC) $(FLAGS) -o gram3 gram3.c time.c $(LIBS)	
	
helloworld: helloworld.c
	$(CC) $(FLAGS) -o helloworld helloworld.c
	
hello2: hello_arg2.c
	$(CC) $(FLAGS) -o hello2 hello_arg2.c

join: join.c
	$(CC) $(FLAGS) -o join join.c $(LIBS)
	
matmul: matmul.c
	$(CC) $(FLAGS) -o matmul matmul.c time.c $(LIBS)	
	
mutex: mutex.c
	$(CC) $(FLAGS) -o mutex mutex.c $(LIBS)	

enumsort: enumsort.c
	$(CC) $(FLAGS) -o enumsort enumsort.c time.c $(LIBS)	

enum1: enumsort_1.c
	$(CC) $(FLAGS) -o enum1 enumsort_1.c time.c $(LIBS)	
	
enum2: enumsort_2.c
	$(CC) $(FLAGS) -o enum2 enumsort_2.c time.c $(LIBS)		

barrier.o: barrier.cc barrier.h 
	$(CPP) $(FLAGS) -c barrier.cc

func.o: func.cc wave.h
	$(CPP) $(FLAGS) -c func.cc

leapfrog.o: leapfrog.cc wave.h
	$(CPP) $(FLAGS) -c  leapfrog.cc

quicksort: quicksort.c 
	$(CC) $(FLAGS) -o quicksort quicksort.c $(LIBS)

clean:
	rm -f *.o *~ wave gram1 gram2 gram3 helloworld hello2 data \
	mutex join enumsort enum1 enum2 synch spinwait pi matmul quicksort



F90C	= f90
CC	= gcc   
FFLAGS    = -O3 -xopenmp
CFLAGS    =  -O3 -fopenmp

helloworld:	helloworld.c
	$(CC) $(CFLAGS) -o helloworld helloworld.c $(LIBS)

datasharing:	datasharing.c
	$(CC) $(CFLAGS) -o datasharing datasharing.c $(LIBS)

loop:	loop.c time.o
	$(CC) $(CFLAGS) -o loop loop.c time.o $(LIBS)

reduce:	reduce.c
	$(CC) $(CFLAGS) -o reduce reduce.c $(LIBS)

pi:	pi.c
	$(CC) $(CFLAGS) -o pi pi.c $(LIBS)

enumsort:	enumsort.c time.o
	$(CC) $(CFLAGS) -o enumsort enumsort.c time.o $(LIBS)

lu: lu.f90 walltime.o
	$(F90C) $(FFLAGS) -o lu lu.f90 walltime.o

lulock: lulock.f90 walltime.o
	$(F90C) $(FFLAGS) -o lulock lulock.f90 walltime.o

time.o:	time.c
	$(CC) $(CFLAGS) -c time.c

walltime.o:	walltime.f90
	$(F90C) $(FFLAGS) -c walltime.f90

clean: 
	rm -f *.o lu lulock enumsort pi reduce loop datasharing helloworld

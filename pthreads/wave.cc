#include  "wave.h"

int main(int argc, char **argv)
{
  int Nx,Ny,nthreads;
  FILE *specf=fopen("wave.dat","r");
  (void) fscanf(specf,"%d %d",&Nx,&Ny);
  (void) fscanf(specf,"%d",&nthreads);
  fclose(specf);

  // Threads 
  pthread_t *thread=new pthread_t[nthreads];
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);
  thread_arg *arg=new thread_arg[nthreads];
  thread_barrier *barrier=new thread_barrier[nthreads+1];
  thread_barrier global(nthreads);
  //thr_setconcurrency(nthreads+1);   // FOR SUN!

  // Problem specific parameters
	int asize=(Nx+1)*(Ny+1);
	double *uold=(double*)malloc(asize*sizeof(double));
	double *u=(double*)malloc(asize*sizeof(double));
	double *unew=(double*)malloc(asize*sizeof(double));
	double *du=(double*)malloc(asize*sizeof(double));
    double dt=1.0/(Nx+Ny); 
	int Nt=int(1.0/dt);
    double norm=0.0;

#if BARRIER == 1
	printf("Version: Overlap\n");
#elif BARRIER == 2
	printf("Version: Peer-to-peer\n");
#elif BARRIER ==3
	printf("Version: Synchronize\n");
#else
	printf("Warning: No synchronization defined!\n");
#endif
	
  printf("Problem size     : %d x %d\n", Nx+1, Ny+1);
  printf("Number of threads: %d\n", nthreads);
  printf("Running...\n");

  // Partition over x, Nx/nthreads+1 lines for Nx%nthreads first threads
  int div=(Nx+1)/nthreads;
  int rem=(Nx+1)%nthreads;
  arg[0].x1=0; int i;
  for (i=0; i<nthreads-1; i++){
    arg[i].x2=arg[i].x1+div+((i<rem)? 1: 0)-1;
    arg[i+1].x1=arg[i].x2+1;
  }
  arg[nthreads-1].x2=Nx;

#if BARRIER == 2
  // Set pairwise peer-to-peer synchronization barriers
  if (nthreads==1){  // Make the code homogeneous
    barrier[0].init(&thread[0],&thread[0],0);
    barrier[1].init(&thread[0],&thread[0],1);
    arg[0].barrier1=&barrier[0];
    arg[0].barrier2=&barrier[1];
  }
  else {
    barrier[0].init(&thread[0],&thread[0],0);    
    barrier[1].init(&thread[0],&thread[1],1);
    arg[0].barrier1=&barrier[0];
    arg[0].barrier2=&barrier[1];
    for (int n=1; n<nthreads-1; n++){
      barrier[n+1].init(&thread[n],&thread[n+1],n+1);
      arg[n].barrier1=&barrier[n];
      arg[n].barrier2=&barrier[n+1];
    }
    barrier[nthreads].init(&thread[nthreads-1],&thread[nthreads-1],nthreads);
    arg[nthreads-1].barrier1=&barrier[nthreads-1];
    arg[nthreads-1].barrier2=&barrier[nthreads];
  }    
#endif
    
  // Solve the problem
  int t1,t2;
  t1=timer();
  for (i=0; i<nthreads; i++){
	arg[i].global=&global;
    arg[i].uold=uold; arg[i].u=u; arg[i].unew=unew; arg[i].du=du;
    arg[i].Nt=Nt; arg[i].dt=dt;
	arg[i].y1=0; arg[i].y2=Ny; arg[i].id=i;
	arg[i].Ny=Ny;arg[i].Nx=Nx;

	  printf("%d \t %d \t %d \t %d\n", arg[i].x1, arg[i].x2, 
		   arg[i].y1, arg[i].y2);
	if (i==nthreads-1) leapfrog((void*) &arg[i]);
	else pthread_create( &thread[i], &attr, leapfrog, (void*) &arg[i]);
  }

  // Master waits for the threads to return
  for (i=0; i<nthreads; i++){
    if (i<nthreads-1) pthread_join(thread[i],NULL);
    norm+=arg[i].norm;
  }
  t2=timer();
  printf("Error norm = %g\n",sqrt(norm/Nx/Ny));
  printf("Run time   = %f\n",(t2-t1)/1000000.0);

  pthread_attr_destroy(&attr);
  return 0;
}


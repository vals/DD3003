#include  "wave.h"

void *leapfrog(void *ptr)
{    
  thread_arg *arg=(thread_arg*) ptr;
  thread_barrier *barrier1=arg->barrier1;
  thread_barrier *barrier2=arg->barrier2;
  thread_barrier *barrier=arg->global;
  double *uold=arg->uold;
  double *u=arg->u;
  double *unew=arg->unew;
  double *du=arg->du;
  double *temp;
  int Nt=arg->Nt; double dt=arg->dt;
  int x1=arg->x1,x2=arg->x2,y1=arg->y1,y2=arg->y2,thrid=arg->id;
  int Nx=arg->Nx, Ny=arg->Ny;

  int i,j,k,locstep=0;
  double x,y,t;
  int len=Ny+1;

   // Initial data
  for (i=x1; i<=x2; i++)
    for (j=y1; j<=y2; j++)
      {
	x=double(i)/Nx; y=double(j)/Ny;
	u[i*len+j]=h(x+y)+up(x,y);
	unew[i*len+j]=h(x+y-2*dt)+up(x,y);
      }

  int tdifftot=0,tdiff;
  int tupdatetot=0,tupdate;
  // Leap-frog
  for (k=2; k<=Nt; k++)
    {
      temp=uold; uold=u; u=unew; unew=temp;
	  tdiff=timer();
#if BARRIER == 2
      barrier1->setpeer();
	  barrier2->setpeer();
#elif BARRIER == 1
	  barrier->start(locstep);	
#elif BARRIER == 3
	  barrier->wait();
#endif
		
		// Inner points 
	  for (i=x1+1; i<x2; i++)
		  for (j=y1+1; j<y2; j++){
			du[i*len+j]=(u[(i+1)*len+j]-u[(i-1)*len+j])/2.0*Nx+(u[i*len+j+1]-u[i*len+j-1])/2.0*Ny;
			x=double(i)/Nx; y=double(j)/Ny;
	        unew[i*len+j]=uold[i*len+j]+2*dt*(F(x,y)-du[i*len+j]);
		  }
			tdifftot+=timer()-tdiff;
		 tupdate=timer();
		
	   // Partition boundary points
#if BARRIER == 2
		barrier1->waitpeer();  
#elif BARRIER == 1
		barrier->end(locstep);
#endif
		if (x1>0)
			for (j=y1+1; j<y2; j++){
				du[x1*len+j]=(u[(x1+1)*len+j]-u[(x1-1)*len+j])/2.0*Nx+(u[x1*len+j+1]-u[x1*len+j-1])/2.0*Ny;
				x=double(x1)/Nx; y=double(j)/Ny;
				unew[x1*len+j]=uold[x1*len+j]+2*dt*(F(x,y)-du[x1*len+j]);
			}
#if BARRIER == 2
		barrier2->waitpeer(); 
#endif
		if (x2<Nx)
			for (j=y1+1; j<y2; j++){
				du[x2*len+j]=(u[(x2+1)*len+j]-u[(x2-1)*len+j])/2.0*Nx+(u[x2*len+j+1]-u[x2*len+j-1])/2.0*Ny;
				x=double(x2)/Nx; y=double(j)/Ny;
				unew[x2*len+j]=uold[x2*len+j]+2*dt*(F(x,y)-du[x2*len+j]);
			}
		
      tupdatetot+=timer()-tupdate;
      t=k*dt; 
      bound(unew,uold,t,dt,x1,x2,y1,y2,Nx,Ny);
    }
	printf("thrid: %d\t Inner: %f \t Boundary: %f\n",thrid,tdifftot/1000000.0, 
		   tupdatetot/1000000.0);

  // Residual
  t=Nt*dt;
  arg->norm=residual(unew,t,x1,x2,y1,y2,Nx,Ny);

  return 0;
}

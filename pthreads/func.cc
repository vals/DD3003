/* Function definition file, 
   includes: bound, h, F, up, residual */

#include "wave.h"

double h(double z){ return sin(2*pi*z); }
double F(double x, double y) //{return 0;}
{ return 2*exp(x+y)+3*x*x+6*y*y+sin((x+y)/2)+cos((x+y)/2); }
double up(double x, double y) //{return 0;}
{ return exp(x+y)+x*x*x+2*y*y*y+sin((x+y)/2)-cos((x+y)/2); }

double residual(double *u,double t,
				int x1, int x2, int y1, int y2, int Nx, int Ny)
{
	int i,j,len=Ny+1;
	double sum,x,y,v;
	
	sum=0.0; 
	for (i=x1; i<=x2; i++)
		for (j=y1; j<=y2; j++)
		{
			x=double(i)/Nx; y=double(j)/Ny;
			v=h(x+y-2*t)+up(x,y);
			sum=sum+(u[i*len+j]-v)*(u[i*len+j]-v);
		}
	return sum;
}


void bound(double *unew, double *uold, double t, double dt,
	   int x1, int x2, int y1, int y2, int Nx, int Ny)
{
  int i,j,len=Ny+1;
  double x,y;

	// Physical boundary conditions
  if (x1==0)
    for (j=y1; j<=y2; j++) 
      {
	y=double(j)/Ny; 
	unew[j]=h(y-2*t)+up(0.0,y);
      }
  if (y1==0)
    for (i=x1; i<=x2; i++)
      {
	x=double(i)/Nx;
	unew[i*len]=h(x-2*t)+up(x,0.0);
      }
  
  /*
  // Numerical boundary conditions (2:nd order if 2dt=dx=dy)
  // Interpolation along the characteristic lines
  if (x2==Nx){
    x=1.0-0.5/Nx; 
    for (j=y1+1; j<=y2; j++) 
      {
	y=(j-0.5)/Ny;
	unew(Nx,j)=uold(Nx-1,j-1)+2*dt*F(x,y);
      }
  }
  if (y2==Ny){
    // Exclude end points
    int xstart=(1<x1? x1:1);
    int xstop=(Nx>x2? x2:Nx-1);

    y=1.0-0.5/Ny;
    for (i=xstart; i<=xstop; i++)
      {
	x=(i-0.5)/Nx; 
	unew(i,Ny)=uold(i-1,Ny-1)+2*dt*F(x,y);
      }
  }
  */

  // Exact boundary conditions
  if (x2==Nx)
    for (j=y1; j<=y2; j++) 
      {
	x=1.0;y=double(j)/Ny; 
	unew[Nx*len+j]=up(x,y)+h(x+y-2*t);
      }
  if (y2==Ny){
    int xstart=(1<x1? x1:1);
    int xstop=(Nx>x2? x2:Nx-1);
    for (i=xstart; i<=xstop; i++)
      {
	y=1.0;x=double(i)/Nx; 
	unew[i*len+Ny]=up(x,y)+h(x+y-2*t);
      }
  }
  
}

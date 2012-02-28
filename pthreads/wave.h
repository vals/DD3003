/* 
 Author: Jarmo Rantakokko     
 Information Technology
 Uppsala University        
 */

/* Define type of barrier
 1:overlap 3:synchronize, 0:no synchronization */ 
#define BARRIER 3


#ifndef wave_h
#define wave_h

#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "barrier.h"

struct thread_arg{
	thread_barrier *barrier1;
	thread_barrier *barrier2;
	thread_barrier *global;
	double *uold;
	double *u;
	double *unew;
	double *du;
	int x1,x2,y1,y2;
	int Nt,id,Nx,Ny;
	double dt;
	double norm;
};

const double pi=3.14159265358979;
double h(double z);
double F(double x, double y);
double up(double x, double y);
void *leapfrog(void*);
void bound(double *unew, double *uold, double t, double dt,
	   int x1, int x2, int y1, int y2, int Nx, int Ny);
double residual(double *u,double t,
		int x1, int x2, int y1, int y2, int Nx, int Ny);
int timer(void);


#endif

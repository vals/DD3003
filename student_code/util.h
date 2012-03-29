/*
 *  util.h
 *  OpenCL Tutorial
 *
 *  Created by David Black-Schaffer on 07/07/2011.
 *  Copyright 2011 Uppsala University. All rights reserved.
 *
 */

#include <sys/time.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "parameters.h"

// Timer structure
#ifndef perf_defined
#define perf_defined 1
typedef struct perf_struct {
	int current_sample;
	double samples[MAX_PERF_SAMPLES];
	struct timeval tv;
} perf;
#endif

// Compute
float find_range(float *data, int size);

// Timer functions
void init_perf(perf *t);
void start_perf_measurement(perf *t);
void stop_perf_measurement(perf *t);
int get_number_of_samples(perf *t);
void print_perf_measurement(perf *t);
double get_total_perf_time(perf *t);
double get_average_perf_time(perf *t);

// Utility functions
void swap(float **in, float **out);			// Swap two float array pointers
void create_data(float **in, float **out);	// Create the data matricies
void print_data(float *data);				// Print out the data for debugging

/*
 *  util.c
 *  OpenCL Tutorial
 *
 *  Created by David Black-Schaffer on 07/07/2011.
 *  Copyright 2011 Uppsala University. All rights reserved.
 *
 */

#include "util.h"


// Swap array pointer function
void swap(float **in, float **out) {
	float *temp;
	temp = *in;
	*in = *out;
	*out = temp;
}



// Find range function
float find_range(float *data, int size) {
	float max, min;
	max = min = 0.0f;
	// Iterate over the data and find the min/max
	for (int i=0; i<size; i++) {
		if (data[i] < min)
			min = data[i];
		else if (data[i] > max)
			max = data[i];
	}
	// Report the range
	return (max-min);
}


// Data utility functions
void create_data(float **in, float **out) {
	printf("Allocating data 2x (%dx%d float, %2.1fMB).\n", SIZE, SIZE, (sizeof(float)*SIZE*SIZE/1024.0/1024.0));
	
	*in = (float*)malloc(SIZE_BYTES);
	*out = (float*)malloc(SIZE_BYTES);
	memset(*in, 0, SIZE_BYTES);
	memset(*out, 0, SIZE_BYTES);
	
	srand(0);
	for (int y=1; y<SIZE-1; y++) {
		for (int x=1; x<SIZE-1; x++) {
			(*in)[SIZE*y+x] = (((float)rand()/RAND_MAX)-0.5)*LIMIT*INITIAL_RANGE_MULTIPLE;
		}
	}
	printf("Range starts as: %f.\n", find_range(*in, SIZE*SIZE));	
}

void print_data(float *data) {
	for (int y=0; y<SIZE; y++) {
		for (int x=0; x<SIZE; x++) {
			printf("[%2.1f]",data[SIZE*y+x]);
		}
		printf("\n");
	}
}



// Performance functions
void init_perf(perf *t) {
	t->current_sample = 0;
}

void start_perf_measurement(perf *t) {
	gettimeofday(&(t->tv),NULL);
}

void stop_perf_measurement(perf *t) {
	struct timeval ctv;
	gettimeofday(&ctv, NULL);
	struct timeval diff;
	timersub(&ctv, &(t->tv), &diff);
	double time = diff.tv_sec + diff.tv_usec/1000000.0;
	t->samples[t->current_sample++] = time;
	if (t->current_sample >= MAX_PERF_SAMPLES) {
		printf("Too many samples.");
		t->current_sample = MAX_PERF_SAMPLES-1;
	}
}

int get_first_index(perf *t) {
	if (PERF_IGNORE_FIRST_LAST && t->current_sample>2)
		return 1;
	return 0;
}

int get_last_index(perf *t) {
	if (PERF_IGNORE_FIRST_LAST && t->current_sample>2)
		return t->current_sample-1;
	return t->current_sample;
}

int get_number_of_samples(perf *t) {
	if (PERF_IGNORE_FIRST_LAST && t->current_sample>2)
		return t->current_sample-2;
	return t->current_sample;
}

void print_perf_measurement(perf *t) {
	double total_time = 0.0;
	double average_time = 0.0;
	double stdev = 0.0;
	
	if (get_number_of_samples(t) == 0) {
		printf("0 samples.\n");
		return;
	}
	
	total_time = get_total_perf_time(t);
	average_time = total_time/get_number_of_samples(t);
	for (int i=get_first_index(t); i<get_last_index(t); i++) {
		stdev += (t->samples[i]-average_time)*(t->samples[i]-average_time);
	}
	stdev /= get_number_of_samples(t);
	stdev = sqrt(stdev);
	
	printf("Total: %f ms\t\t(Avg: %f ms, Std. dev.: %f ms, %d samples)",
		   total_time*1000, average_time*1000, stdev*1000, t->current_sample);

//	printf("Avg: %f ms, Total: %f ms, Std. dev.: %f ms, %d samples.",
//		   average_time*1000, total_time*1000, stdev*1000, t->current_sample);
	
	// Calculate the average throwing out outliers
//	double average_less_than_sdev = 0.0;
//	int number_less_than_sdev = 0;
//	for (int i=0; i<t->current_sample; i++) {
//		if (fabs(average_time-(t->samples[i])) < OUTLIER_STD_DEV*stdev) {
//			average_less_than_sdev+=t->samples[i];
//			number_less_than_sdev++;
//		}
//	}
//	average_less_than_sdev /= number_less_than_sdev;
//	printf(" [Avg. for all less than 1 stdev: %f ms (%d)]", average_less_than_sdev*1000, number_less_than_sdev);
	
//	printf("Average: %f ms (ignoring outliers > %d std. dev.: %d samples used of %d total)\n     Raw Average: %f ms, Total: %f ms, Std. dev: %f ms.",
//		   average_less_than_sdev*1000, OUTLIER_STD_DEV, number_less_than_sdev, t->current_sample, average_time*1000, total_time*1000, stdev*1000);
	
	
	if (PERF_IGNORE_FIRST_LAST && t->current_sample>2)
		printf(" [ignoring first/last samples]\n");
	else
		printf("\n");
}

double get_average_perf_time(perf *t) {
	return get_total_perf_time(t)/get_number_of_samples(t);
}

double get_total_perf_time(perf *t) {
	double total_time = 0.0;
	for (int i=get_first_index(t); i<get_last_index(t); i++) {
		total_time += t->samples[i];
	}
	return total_time;
}


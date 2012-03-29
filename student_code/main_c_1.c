#include "parameters.h"
#include "util.h"



// Compute functions
void update(float *in, float *out);			// Update stencil calculation

// Performance measurement
perf total_perf, update_perf, range_perf;
void init_all_perfs();
void print_perfs();


int main (int argc, const char * argv[]) {
	printf("1. C\n");
	
	float range = BIG_RANGE;
	float *in, *out;
	int iterations = 0;

	// ======== Initialize
	init_all_perfs();
	create_data(&in, &out);
	start_perf_measurement(&total_perf);

	
	// ======== Compute
	while (range > LIMIT) {
		// Calculation
		start_perf_measurement(&update_perf);
		update(in, out);
		stop_perf_measurement(&update_perf);
		
		// Compute Range
		start_perf_measurement(&range_perf);
		range = find_range(out, SIZE*SIZE);
		stop_perf_measurement(&range_perf);

		iterations++;
		swap(&in, &out);

		printf("Iteration %d, range=%f.\n", iterations, range);
	}	
	
	stop_perf_measurement(&total_perf);
	print_perfs();
	
	free(in);
	free(out);
}



// Update function
void update(float *in, float *out) {
	for (int y=1; y<SIZE-1; y++) {
		for (int x=1; x<SIZE-1; x++) {
			float a = in[SIZE*(y-1)+(x)];
			float b = in[SIZE*(y)+(x-1)];
			float c = in[SIZE*(y+1)+(x)];
			float d = in[SIZE*(y)+(x+1)];
			float e = in[SIZE*y+x];
			out[SIZE*y+x] = (0.1*a+0.2*b+0.2*c+0.1*d+0.4*e);
		}
	}
}






void init_all_perfs() {
	init_perf(&total_perf);
	init_perf(&update_perf);
	init_perf(&range_perf);
}

void print_perfs() {
	printf("Total:  ");
	print_perf_measurement(&total_perf);
	printf("Update: ");
	print_perf_measurement(&update_perf);
	printf("Range:  ");
	print_perf_measurement(&range_perf);
}	

